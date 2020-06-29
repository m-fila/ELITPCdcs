#open62541_path=/home/mfila/open62541-install
open62541_path=/opt/soft/open62541-v1.1
export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:${open62541_path}
export open62541_LIB=${open62541_path}/lib