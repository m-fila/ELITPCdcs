#open62541_path=/home/mfila/open62541-install
open62541_path=/opt/soft/open62541-1.1rc
export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:${open62541_path}
export open62541_LIB=${open62541_path}/lib