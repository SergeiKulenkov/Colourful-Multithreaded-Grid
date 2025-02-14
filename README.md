# Colourful Multithreaded Grid
 
### Created using a C++ 20 thread pool and ImGUI (based on my previously used project setup)

### Features ImGUI window with:
* text fields showing grid and cell size
* checkbox that allows to enable/disable the thread pool
* start/stop button for updates
* text field with a timer for all grid updates

### After the user presses the Start button Grid class starts applying random changes (changing colours in a specified range and moving the rows).
### So all of these random calculations are much faster with thread pool--0.6ms vs 6ms in my testing with 150x150 grid.

*Run Setup-Windows.bat to create Visual Studio project files and solution.*