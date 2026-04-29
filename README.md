
# Automobile Insurance Management System


A Windows-based, text-driven automobile insurance management system built in C++17 with a clear 3-layer architecture.

## Overview

This project models the day-to-day operations of an automobile insurance company. It stores data in flat text files, keeps business logic separate from the user interface, and supports role-based workflows for managers, salesmen, and surveyors.

## Features

- Customer, vehicle, policy, claim, inspection, workshop, and staff management
- Role-based login for Manager, Salesman, and Surveyor
- File-based persistence with no DBMS dependency
- Automatic creation of the `data/` folder on first run
- ANSI-colored console UI for a cleaner terminal experience

## Build And Run

This project is meant to be built from [main.cpp](main.cpp) together with the rest of the source files.

On Windows with MinGW / GCC:

```bash
g++ -std=c++17 -Wall -o insurance.exe main.cpp business/*.cpp repository/*.cpp ui/*.cpp
.\insurance.exe
```

If you are using VS Code, you can also use the Run button after configuring your GCC toolchain.

## Default Login Credentials

| Role | Username | Password |
| --- | --- | --- |
| Manager | admin | admin123 |
| Salesman | bilal | pass123 |
| Surveyor | sara | pass123 |

## Architecture

- `business/`: entity classes and the `InsuranceSystem` facade
- `repository/`: file I/O layer for flat-text persistence
- `ui/`: menu-driven console interfaces for each role
- `main.cpp`: application entry point

## Project Structure

```
├── main.cpp
├── business/
├── repository/
├── ui/
├── data/
├── class_diagram.png
├── use_case_diagram.png
└── README.md
```

## Workflow

1. Salesman registers customers, adds vehicles, issues policies, and files claims.
2. Surveyor reviews assigned claims and submits inspection reports.
3. Manager reviews inspections and approves or rejects claims.

## Notes

- `insurance.exe` is a generated build artifact and should not be committed.
- The repository includes project diagrams for the class model and use-case flow.
