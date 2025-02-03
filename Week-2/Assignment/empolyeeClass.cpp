class Employee {
    int id;
    std::string name;
    std::string department;
    bool working;

public:
    void terminateEmployee();
    bool isWorking();
};

class EmployeeStorage {
public:
    void saveEmployeeToDatabase(const Employee& emp);
};

class IReportGenerator {
public:
    virtual void printEmployeeDetailReport(const Employee& emp) const = 0;
};

class XMLReport : public IReportGenerator {
public:
    void printEmployeeDetailReport(const Employee& emp) const override;
};

class CSVReport : public IReportGenerator {
public:
    void printEmployeeDetailReport(const Employee& emp) const override;
};
