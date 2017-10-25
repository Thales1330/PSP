.PHONY: clean All

All:
	@echo "----------Building project:[ Project - Release_Ubuntu_x64 ]----------"
	@cd "Project" && "$(MAKE)" -f  "Project.mk"
clean:
	@echo "----------Cleaning project:[ Project - Release_Ubuntu_x64 ]----------"
	@cd "Project" && "$(MAKE)" -f  "Project.mk" clean
