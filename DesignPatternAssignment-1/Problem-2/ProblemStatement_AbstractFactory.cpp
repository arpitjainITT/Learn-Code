/*


Problem Statement: Cross-Platform UI Component Library -

You are developing a Cross-Platform UI Component Library that supports two operating systems: Windows and MacOS. Each OS has its own look and feel for UI components such as Buttons, Checkboxes, and TextFields.
The system must allow clients to create UI elements without knowing the exact classes of the components that will be created, and ensure that the components created together are from the same family (i.e., all Windows-style or all MacOS-style).


Requirements:

a. Ensure the client code can work with UI components from either family without knowing the platform-specific details.
b. Implement the Abstract Factory Design Pattern to allow the creation of related UI components as families, supporting platform scalability and ensuring consistent UI appearance.


*/


//Command to run g++ main/main.cpp src/Application.cpp src/MacOSComponents.cpp src/MacOSUIFactory.cpp src/WindowsComponents.cpp src/WindowsUIFactory.cpp -Iinc -o operating_system_UI_rendering