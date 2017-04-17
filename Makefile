.PHONY: clean All

All:
	@echo "----------Building project:[ Project - Release ]----------"
	@cd "Project" && "$(MAKE)" -f  "Project.mk"
clean:
	@echo "----------Cleaning project:[ Project - Release ]----------"
	@cd "Project" && "$(MAKE)" -f  "Project.mk" clean
