# Documentation
To build documentation you will need `doxygen` and `sphinx` with extensions `rtd_theme`, `breath` and `exhale`.
```
sudo apt-get install doxygen python-sphinx python-sphinx-breathe python-sphinx_rtd_theme
pip install exhale
```
To build documentation run
```
cd build
cmake ..
make docs
```
Documentation will be available at `build/docs/html/index.html` . 

