# ZPO-Histogram

## How to create environment to run the application
The application is developed against Python3.

The application also has some external dependencies that
need to be installed in order to run. This section describes
how to prepare virtual environment, which will be isolated
from system python environment, so that installing these
dependencies won't pollute system. 

### Install Python3 (prerequisite)
The application requires Python3 to run. You can download
and install latest version from `https://www.python.org/downloads/`.
Alternatively, you can use Linux package managers. 

### Install virtualenv package (prerequisite)
Following instructions require that `virtualenv` package is
installed for Python3 on the system. Usually, it is already
installed with Python3 but it can be also additionaly installed
with `pip install virtualenv`.

### Create new virtual environment in project folder
Change directory to project folder and run:
`virtualenv --no-site-packages venv`.
This will create new python environment which will be further used 
so that system environment remains unchanged.

### Activate virtual environment
The virtual enivronment is activated with one of scripts that were created
inside of the `venv` folder.
For Windows, run `venv\Scripts\activate` from project folder. The name
of virtual environment should appear before the path in command prompt,
e.g. `(venv) E:\GitHub\ZPO-Histogram`.

### Install dependencies
#### Linux / Mac
With activated virtual enivornment run: `pip install -r require.txt`

You are now ready to run the application.

#### Windows
With activated virtual enivornment run: `pip install -r require_win.txt`.
Unfortunately, for Windows there are currently no official precompiled 
numpy+mkl and SciPy packages so they need to be installed manually. 
Please download unnoficial wheel package for 
[numpy 1.11.3](http://www.lfd.uci.edu/~gohlke/pythonlibs/#numpy)
and
[scipy 0.19.0](http://www.lfd.uci.edu/~gohlke/pythonlibs/#scipy)
for your version of Windows and Python (e.g. 
`scipy-0.19.0-cp35-cp35m-win_amd64.whl` for Windows x64 and Python 3.5).
Place the files inside project folder and install them individually 
with `pip` like this `pip install scipy-0.19.0-cp35-cp35m-win_amd64.whl`
(don't forget to change the filename).

You are now ready to run the application.

### Deactivate virtual environment
If you are finished working with virtual environment deactivate it 
with `deactivate` command. The name of virtual environment should
disappear.

## How to run the application
Change to project folder and activate your virtual environment. Then
just run the main script file with `python main.py`.
