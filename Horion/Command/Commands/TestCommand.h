#pragma once
#include "ICommand.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>

class TestCommand :
	public ICommand
{
public:
	TestCommand();
	~TestCommand();

	// Inherited via ICommand
	virtual bool execute(std::vector<std::string>* args) override;
};

