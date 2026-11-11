# Hospital Patient Management System (C)

A console application in C that manages patients, doctors and a waiting line in a hospital ward. Advanced C course project at Ruppin Academic Center.

## Data structures
- **Binary search tree** (`ptree`) of patients, keyed by ID
- **Linked list queue** (`pline`) for the patient waiting line
- **Stack of visits** (`visit`) per patient
- **Dynamic array** of doctors (`doc`), assigned by current load

## Features
- Load and save patients, doctors and the line from text files
- Log a new visit and assign an available doctor automatically
- Discharge patients, remove visits and remove patients
- Free all dynamic memory on exit

## Project structure
| File | Purpose |
|------|---------|
| `main.c` | Menu and program flow |
| `patient.c/h`, `ptree.c/h` | Patient records and BST |
| `pline.c/h` | Waiting line |
| `visit.c/h`, `date.c/h` | Visits and dates |
| `doc.c/h` | Doctors |
| `*.txt` | Sample data (fictional) |

## Build & run
Open `kusayProj.sln` in Visual Studio, or:
```bash
gcc *.c -o hospital
./hospital
```

## Authors
Adam Elmalek, Qusai Darwish
