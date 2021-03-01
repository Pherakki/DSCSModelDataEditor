# DSCSModelDataEditor
Software to edit and visualise the data and shaders of models for Digimon Story: Cyber Sleuth.

Assigning Cg shaders to DSCS models is not an easy or elegant task with Blender, and so a second tool would be helpful in this regard. Given that the model-reading code is in Python and developing with Python is probably a lot quicker than C++ with OpenGL, the current idea is to use [Panda3D](https://www.panda3d.org/) as a rendering engine to display the models and shaders. I'm busy with several projects and will take a while to get around to this; so in the meantime this repository has been opened in case anybody would like to contribute any of the following (plus anything else that would be helpful!) to the repository to speed things along:

* A module to convert [the geometry, skeleton, and animations of this intermediate format class](https://github.com/Pherakki/Blender-Tools-for-DSCS/blob/master/CollatedData/IntermediateFormat.py) to an [.egg file](https://docs.panda3d.org/1.10/python/tools/model-export/egg-syntax)
* A module to replace variable names in the DSCS shader files with [variable names recognised by Panda3D](https://docs.panda3d.org/1.10/python/programming/shaders/list-of-cg-inputs) (can probably be circumvented and less error-prone to just pass all variables manually, if possible, so this item is not a strict necessity)
* A Panda3D render window which can swap Cg shader inputs in real time when selected from some control (possibly integrated with PyQt5?)

Note that this repository may be deleted if a better solution is found. My inbox and the discussions tab on this repository are both open for communications. :)
