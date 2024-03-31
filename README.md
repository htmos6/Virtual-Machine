# Virtual-Machine

To run the program with a command-line argument in Visual Studio and debug it with command-line parameters, follow these steps:

1 - Open Project Properties: Right-click on the project node in the Solution Explorer and select "Properties".

2 - Navigate to Debugging Settings: In the project properties window, go to the "Debug" tab or section.

3 - Specify Command-Line Arguments: Look for the "Command Arguments" field within the Debugging settings. This is where you can provide command-line arguments to your program.

4 - Enter Command-Line Argument: In the "Command Arguments" field, enter the command-line argument you want to use. For example, if you want to provide -2048.obj as an argument, simply type -2048.obj into the field.

5 - Save Changes: After entering the command-line argument, save the changes to the project properties.

6 - Debug the Program: Now, when you debug your program in Visual Studio, it will automatically pass the specified command-line argument (-2048.obj in this case) to your program.

By setting the command-line arguments in the project properties, Visual Studio will pass these arguments to your program every time you run it in debug mode, allowing you to debug your program with the specified command-line parameters.