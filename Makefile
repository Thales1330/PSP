.PHONY: clean All

All:
	@echo "----------Building project:[ Project - Debug ]----------"
	@cd "Project" && "$(MAKE)" -f  "Project.mk"
clean:
	@echo "----------Cleaning project:[ Project - Debug ]----------"
	@cd "Project" && "$(MAKE)" -f  "Project.mk" clean
