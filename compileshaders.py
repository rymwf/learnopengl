import os
import sys
import getopt

PATH_SHADER = "./shaders"
PATH_SHADER_COMPILE_DST = "./shaders"

def compileShader(filename):
    if os.path.splitext(filename)[1] == ".spv":
        return
    os.system('cmd /c "glslc ' + filename + " -o " +
              PATH_SHADER_COMPILE_DST+"/"+os.path.basename(filename)+".spv" + '" --target-env=opengl')

def processArg(argv):
    if not os.path.exists(PATH_SHADER_COMPILE_DST):
        os.mkdir(PATH_SHADER_COMPILE_DST)
    if len(argv) == 0:
        shaders = os.listdir(PATH_SHADER)
        for f in shaders:
            compileShader(PATH_SHADER+"/" + f)
    else:
        for f in argv:
            compileShader(f)

if __name__ == "__main__":
    processArg(sys.argv[1:])
