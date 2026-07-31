#include <stdio.h>
#include <cmath>
#include <string.h>
#include <windows.h>
#include <stdint.h>

class PhasorEstimator
{
public:

    PhasorEstimator(double sampleRate = 1e6, double notchRadius = 0.5) : Fs(sampleRate), r(notchRadius)
    {
        Reset();
        UpdateFrequency(60.0);
    }

    void Reset()
    {
        x1Id = x2Id = 0.0;
        y1Id = y2Id = 0.0;

        x1Iq = x2Iq = 0.0;
        y1Iq = y2Iq = 0.0;

        Id = 0.0;
        Iq = 0.0;
		
		lastFreq = -1.0;
    }

    void UpdateFrequency(double freq)
    {
        if(std::abs(freq-lastFreq) < 1e-9)
            return;

        lastFreq = freq;

        double w0 = 2.0*M_PI*(2.0*freq)/Fs;

        double c = cos(w0);

        double bb0 = 1.0;
        double bb1 = -2.0*c;
        double bb2 = 1.0;

        a1 = -2.0*r*c;
        a2 = r*r;

        double gain =
            (1.0+a1+a2)/
            (bb0+bb1+bb2);

        b0 = gain*bb0;
        b1 = gain*bb1;
        b2 = gain*bb2;
    }

    void Update(double current,
                double phase,
                double freq)
    {
        UpdateFrequency(freq);

        double s = sin(phase);
        double c = cos(phase);

        double IdRaw = current*s;
        double IqRaw = current*c;

        //-----------------------------------
        // Id
        //-----------------------------------

        Id = b0*IdRaw + b1*x1Id + b2*x2Id - a1*y1Id - a2*y2Id;

        x2Id = x1Id;
        x1Id = IdRaw;

        y2Id = y1Id;
        y1Id = Id;

        //-----------------------------------
        // Iq
        //-----------------------------------

        Iq = b0*IqRaw + b1*x1Iq + b2*x2Iq - a1*y1Iq - a2*y2Iq;

        x2Iq = x1Iq;
        x1Iq = IqRaw;

        y2Iq = y1Iq;
        y1Iq = Iq;
    }

    double GetId() const
    {
        return sqrt(2.0)*Id;
    }

    double GetIq() const
    {
        return sqrt(2.0)*Iq;
    }

    double GetMagnitude() const
    {
        double ir = GetId();
        double ii = GetIq();

        return sqrt(ir*ir+ii*ii);
    }

    double GetAngle() const
    {
        return atan2(GetIq(),GetId());
    }

private:

    double Fs;
    double r;

    double lastFreq;

    double b0,b1,b2;
    double a1,a2;

    //-----------------------------
    // Id
    //-----------------------------

    double x1Id,x2Id;
    double y1Id,y2Id;

    //-----------------------------
    // Iq
    //-----------------------------

    double x1Iq,x2Iq;
    double y1Iq,y2Iq;

    //-----------------------------

    double Id;
    double Iq;
};

enum Mode
{
    RAW_CURRENT = 0,
    PHASOR = 1,
    BOTH = 2
};

struct Sample
{
    double current[3];
};

struct Phasor
{
    double Id[3];
    double Iq[3];
};

// Shared memory structure used for data exchange between PSP and ATP.
struct SharedData
{
    uint32_t nPh;       // Number of phases

    double t;           // Current simulation time

    double vrms;        // RMS voltage at the PSP bus
    double freq;        // System frequency at the PSP bus
    double theta;       // Voltage phase angle at the PSP bus
    double phase;       // Continuous reference angle

    double stoptime;    // ATP simulation end time
	double atpStepsize;		// ATP steptime
	double pspStepsize;		// PSP steptime

    Mode mode;      // Data acquisition mode
    int terminate;      // Simulation termination flag

    Phasor phasor;      // Quadrature phasor components

    uint32_t stepCount; // ATP steps since last synchronization
    uint32_t syncSteps; // ATP steps between synchronizations

    Sample samples[];   // Current samples collected between synchronizations
};

HANDLE hPSPReady = NULL;
HANDLE hATPReady = NULL;
HANDLE hMapFile = NULL;
SharedData* data = NULL;

const double PHASE_A = 0.0;
const double PHASE_B = -2.0 * M_PI / 3.0;
const double PHASE_C =  2.0 * M_PI / 3.0;

static PhasorEstimator estimator[3];

const double phaseShift[3] =
{
    PHASE_A,
    PHASE_B,
    PHASE_C
};

inline void SaveCurrent(const double current[])
{
    if(data->stepCount >= data->syncSteps)
    {
        printf("Current buffer full!\n");
        return;
    }

    for(int i = 0; i < data->nPh; ++i)
        data->samples[data->stepCount].current[i] = current[i];
}

inline void UpdatePhasor(const double current[])
{
    for(int i = 0; i < data->nPh; ++i)
    {
        estimator[i].Update(
            current[i],
            data->phase,
            data->freq);

        data->phasor.Id[i] = estimator[i].GetId();
        data->phasor.Iq[i] = estimator[i].GetIq();
    }
}

inline bool ProcessSharedData(const double current[])
{
    switch(data->mode)
    {
    case RAW_CURRENT:
        SaveCurrent(current);
        break;

    case PHASOR:
        UpdatePhasor(current);
		break;
		
    case BOTH:
        SaveCurrent(current);
        UpdatePhasor(current);
        break;
		
	default:
		printf("Invalid acquisition mode.\n");
		return false;
    }
	
	data->stepCount++;
	
	if(data->stepCount < data->syncSteps) // Not yet the time to synchronize with PSP
        return false;

    return true; // Synchronize with PSP
}

extern "C"
void psp_model_m__(double xdata_ar[], double xin_ar[], double xout_ar[], double xvar_ar[])
{ 
	if(data == NULL)
	{
		printf("Shared memory not initialized.\n");
		return;
	}
	//static int step = 0;
    //step++;
		
	data->nPh = xdata_ar[0];
	data->stoptime = xdata_ar[1];
	data->atpStepsize = xdata_ar[2];
	
	double vth[3];
	double rth[3];
	for(int i = 0; i < data->nPh; ++i) {
		vth[i] = xin_ar[i];
		rth[i] = xin_ar[i + data->nPh];
	}
	data->t = xin_ar[2* data->nPh];

	double phase = xvar_ar[0];
	double lastTime = xvar_ar[1];

	double dt = data->t - lastTime;

	if(dt > 0)
	{
		phase += 2*M_PI*data->freq*dt;
		phase = fmod(phase, 2.0*M_PI);
		lastTime = data->t;
	}

	xvar_ar[0] = phase;
	xvar_ar[1] = lastTime;
	
	data->phase = phase;
	
	if (data->t >= data->stoptime - 1e-12) data->terminate = 1;
	else data->terminate = -1;
	
	double v[3];
	for (int i = 0; i < data->nPh; ++i )
		v[i] = data->vrms * sqrt(2.0) * sin(data->phase + data->theta + phaseShift[i]);
	
	double current[3] = {0.0, 0.0, 0.0};
	for(int i = 0; i < data->nPh; ++i) {
		if(fabs(rth[i]) > 1e-12)
			current[i] = (vth[i]-v[i])/rth[i];
		xout_ar[i] = current[i];
	}
	
	if(ProcessSharedData(current))
	{
		if(!SetEvent(hATPReady))
		{
			printf("Error in SetEvent ATP: %lu\n", GetLastError());
		}

		DWORD ret = WaitForSingleObject(hPSPReady, INFINITE);
		
		if(ret == WAIT_OBJECT_0)
		{
			printf("Event received!\n");
		}
		else
		{
			printf("Error in WaitForSingleObject: %lu\n", GetLastError());
		}

		data->stepCount = 0;
	}
	return;
}

extern "C"
void psp_model_i__(double xdata_ar[], double xin_ar[], double xout_ar[], double xvar_ar[])
{ 
	printf("Initializing model 'PSP MODEL'.\n");
	
	hPSPReady = CreateEventW(
        NULL,
        FALSE,      // auto-reset
        FALSE,      // inicialmente não sinalizado
        L"PSPBridge_PSPReady");
		
	if(hPSPReady == NULL)
    {
        printf("Error in CreateEvent: %lu\n", GetLastError());
    }
		
	hATPReady = CreateEventW(
		NULL,
		FALSE,
		FALSE,
		L"PSPBridge_ATPReady");

    if(hATPReady == NULL)
    {
        printf("Error in CreateEvent: %lu\n", GetLastError());
    }
	
	if(!SetEvent(hATPReady))
	{
		printf("Error in SetEvent ATP: %lu\n", GetLastError());
	}
	
	double pspStepsize = xdata_ar[3];
	double atpStepsize = xdata_ar[2];	
	
	const uint32_t syncSteps = static_cast<uint32_t>(std::ceil(pspStepsize / atpStepsize - 1e-12));

	size_t bytes =
		sizeof(SharedData)
		+ syncSteps * sizeof(Sample);

	hMapFile = CreateFileMappingW(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		bytes,
		L"PSPBridgeSharedMemory");

	if(hMapFile == NULL)
	{
		printf("Error in CreateFileMapping: %lu\n", GetLastError());
		return;
	}

	data = (SharedData*)MapViewOfFile(
		hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		bytes);

	if(data == NULL)
	{
		printf("Error in MapViewOfFile: %lu\n", GetLastError());
		CloseHandle(hMapFile);
		return;
	}
	
	for(int i = 0; i < 3; ++i)
		estimator[i].Reset();
	

	memset(data, 0 , bytes);
	data->nPh = xdata_ar[0];
	data->stoptime = xdata_ar[1];
	data->vrms = 0.0;
	data->freq = 0.0;
	data->mode = PHASOR;
	data->terminate = -1;
	data->syncSteps = syncSteps;
	data->stepCount = 0;
	
	xvar_ar[0] = 0.0;
	xvar_ar[1] = 0.0;
	
	printf("Waiting PSP to initialize...\n");

	DWORD ret = WaitForSingleObject(hPSPReady, INFINITE);

	if(ret != WAIT_OBJECT_0)
	{
		printf("Error waiting PSP.\n");
		return;
	}
	
	return;
}