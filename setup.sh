#!/bin/bash
echo "PixelEngine project setup"
echo ""
echo "You should have already done the required steps before running this file."
echo "Please do these first unless you know what you're doing."
echo "Instructions: https://github.com/PieKing1215/PixelEngine/wiki/Building"
echo ""
read -p "Press [Enter] to start (or Ctrl+C to exit)..."
echo ""

echo "Looking for Conan..."
command -v conan >/dev/null 2>&1 || { 
    echo "Conan is an open source C++ package manager, and is needed to set up libraries for PixelEngine."
    echo "Run the Conan installer which you can download at: https://conan.io/downloads.html"
    echo "More instructions: https://docs.conan.io/en/latest/installation.html"
    read -p "Press [Enter] once Conan is installed to continue..."
}

command -v conan >/dev/null 2>&1 || {
    echo "Could not find the \"conan\" command.";
    echo "Maybe the install didn't finish or it wasn't added to the path?";
    echo "Try closing and reopening this console to refresh the PATH if the Conan installer said it was successful.";
    exit 1; 
}

conan --version
echo "Conan is installed."
echo "Setting up Conan remotes..."

if conan remote list | grep bincrafters -q
then
    if conan remote list | grep bincrafters.jfrog.io -q
    then
        echo "Bincrafters Conan repo is already installed."
    else
        echo "Bincrafters Conan repo moved, switching in conan..."
        conan remote remove bincrafters
        conan remote add bincrafters https://bincrafters.jfrog.io/artifactory/api/conan/public-conan
        conan config set general.revisions_enabled=1
        echo "Clearing cache for sdl2_ttf..."
        conan remove sdl2_ttf -f
        echo "Done."
    fi
else
    echo "Adding Bincrafters Conan repo..."
    conan remote add bincrafters "https://bincrafters.jfrog.io/artifactory/api/conan/public-conan"
    conan config set general.revisions_enabled=1
    echo "Done."
fi

echo "Setting up Conan sdl_gpu..."
if conan search sdl_gpu | grep sdl_gpu/ -q
then
    if conan search sdl_gpu | grep sdl_gpu/20201002 -q
    then
        echo "There is a new version of sdl_gpu which needs to be installed."
        
        echo "Cloning github.com/sixten-hilborn/conan-sdl_gpu..."
        git clone https://github.com/sixten-hilborn/conan-sdl_gpu ./conan-sdl_gpu
        cd ./conan-sdl_gpu
        
        # Patches to use newer version of sdl_gpu
        sed -i 's/20171229/20201002/g' ./conanfile.py
        sed -i 's/143f767adf7d472f81ce890d4692ed29369aa8f3/47a3e2b2a9326c33ad6f177794705987399de8cf/g' ./conanfile.py
        sed -i 's/2.0.9/2.0.12/g' ./conanfile.py
        sed -i "s/sdl2 sdl2 sdl-2.0'/SDL2d sdl2 sdl2 sdl-2.0'/g" ./conanfile.py
        sed -i "s/sdl2 sdl2 sdl-2.0 SDL2d'/SDL2d sdl2 sdl2 sdl-2.0'/g" ./conanfile.py
        sed -i "s/SDL_gpu_INSTALL/INSTALL_LIBRARY/g" ./conanfile.py
        sed -i "s/SDL_gpu_BUILD_DEMOS/BUILD_DEMOS/g" ./conanfile.py
        sed -i "s/SDL_gpu_BUILD_SHARED/BUILD_SHARED/g" ./conanfile.py
        sed -i "s/SDL_gpu_BUILD_STATIC/BUILD_STATIC/g" ./conanfile.py
        sed -i "s/SDL_gpu_USE_SDL1/USE_SDL1/g" ./conanfile.py
        sed -i "s/SDL_gpu_DISABLE_GLES_1/DISABLE_GLES_1/g" ./conanfile.py
        conan export . sdl_gpu/20201002
        cd ../
        
        if conan search sdl_gpu | grep sdl_gpu/20201002 -q
        then
            echo "Successfully installed sdl_gpu into Conan."
            
            rm -rf ./conan-sdl_gpu/
        else
            echo "Failed to install sdl_gpu into Conan."
            exit 1; 
        fi
    else
        echo "Conan sdl_gpu already installed."
    fi
else
    echo "Cloning github.com/sixten-hilborn/conan-sdl_gpu..."
    git clone https://github.com/sixten-hilborn/conan-sdl_gpu ./conan-sdl_gpu
    cd ./conan-sdl_gpu
    
    # Patches to use newer version of sdl_gpu
    sed -i 's/20171229/20201002/g' ./conanfile.py
    sed -i 's/143f767adf7d472f81ce890d4692ed29369aa8f3/47a3e2b2a9326c33ad6f177794705987399de8cf/g' ./conanfile.py
    sed -i 's/2.0.9/2.0.12/g' ./conanfile.py
    sed -i "s/sdl2 sdl2 sdl-2.0'/SDL2d sdl2 sdl2 sdl-2.0'/g" ./conanfile.py
    sed -i "s/sdl2 sdl2 sdl-2.0 SDL2d'/SDL2d sdl2 sdl2 sdl-2.0'/g" ./conanfile.py
    sed -i "s/SDL_gpu_INSTALL/INSTALL_LIBRARY/g" ./conanfile.py
    sed -i "s/SDL_gpu_BUILD_DEMOS/BUILD_DEMOS/g" ./conanfile.py
    sed -i "s/SDL_gpu_BUILD_SHARED/BUILD_SHARED/g" ./conanfile.py
    sed -i "s/SDL_gpu_BUILD_STATIC/BUILD_STATIC/g" ./conanfile.py
    sed -i "s/SDL_gpu_USE_SDL1/USE_SDL1/g" ./conanfile.py
    sed -i "s/SDL_gpu_DISABLE_GLES_1/DISABLE_GLES_1/g" ./conanfile.py
    conan export . sdl_gpu/20201002
    cd ../
    
    if conan search sdl_gpu | grep sdl_gpu/20201002 -q
    then
        echo "Successfully installed sdl_gpu into Conan."
        
        rm -rf ./conan-sdl_gpu/
    else
        echo "Failed to install sdl_gpu into Conan."
        exit 1; 
    fi
    
fi

echo ""
echo "Conan should be ready to go."
echo "Make sure you install the Conan Extension for Visual Studio: https://marketplace.visualstudio.com/items?itemName=conan-io.conan-vs-extension".
echo ""
read -p "Press [Enter] to continue to the next step..."
echo ""


# if [ -d "./PixelEngine/lib/FMOD/inc" ] && [ -d "./PixelEngine/lib/FMOD_studio/inc" ] && [ -f "./PixelEngine/lib/bin/x64/fmod.dll" ]
# then
    # echo "FMOD already set up."
# else
    # echo "Setting up FMOD (required):"
    # echo "Go to https://fmod.com/download (you will need to make an account)"
    # echo "Under \"FMOD Studio Suite\"->\"FMOD Engine\"->\"Windows\"->Download"
    # echo "This should get you an installer, so install the FMOD API files (you can uninstall later)."
    # echo ""
    # echo "Go to wherever you installed it to (by default \"C:\\Program Files (x86)\\FMOD SoundSystem\\FMOD Studio API Windows\\\")"
    # echo "Copy the api folder and paste it next to this script (it will be deleted after)."
    # echo ""
    # read -p "Press [Enter] once the api/ folder is placed next to this script..."
    
    # if [ -d "./api" ]
    # then
        # echo "./api/ found, copying files to the right places..."
        # echo "./api/core/inc -> ./PixelEngine/lib/FMOD/inc"
        # cp -r ./api/core/inc ./PixelEngine/lib/FMOD/inc
        # echo "./api/core/lib -> ./PixelEngine/lib/FMOD/lib"
        # cp -r ./api/core/lib ./PixelEngine/lib/FMOD/lib
        
        # echo "./PixelEngine/lib/FMOD/lib/x86/fmod.dll -> ./PixelEngine/lib/bin/x86/fmod.dll"
        # cp ./PixelEngine/lib/FMOD/lib/x86/fmod.dll ./PixelEngine/lib/bin/x86/fmod.dll
        # echo "./PixelEngine/lib/FMOD/lib/x86/fmodL.dll -> ./PixelEngine/lib/bin/x86/fmodL.dll"
        # cp ./PixelEngine/lib/FMOD/lib/x86/fmodL.dll ./PixelEngine/lib/bin/x86/fmodL.dll
        
        # echo "./PixelEngine/lib/FMOD/lib/x64/fmod.dll -> ./PixelEngine/lib/bin/x64/fmod.dll"
        # cp ./PixelEngine/lib/FMOD/lib/x64/fmod.dll ./PixelEngine/lib/bin/x64/fmod.dll
        # echo "./PixelEngine/lib/FMOD/lib/x64/fmodL.dll -> ./PixelEngine/lib/bin/x64/fmodL.dll"
        # cp ./PixelEngine/lib/FMOD/lib/x64/fmodL.dll ./PixelEngine/lib/bin/x64/fmodL.dll
        
        # echo "./api/studio/inc -> ./PixelEngine/lib/FMOD_studio/inc"
        # cp -r ./api/studio/inc ./PixelEngine/lib/FMOD_studio/inc
        # echo "./api/studio/lib -> ./PixelEngine/lib/FMOD_studio/lib"
        # cp -r ./api/studio/lib ./PixelEngine/lib/FMOD_studio/lib
        
        # echo "./PixelEngine/lib/FMOD_studio/lib/x86/fmodstudio.dll -> ./PixelEngine/lib/bin/x86/fmodstudio.dll"
        # cp ./PixelEngine/lib/FMOD_studio/lib/x86/fmodstudio.dll ./PixelEngine/lib/bin/x86/fmodstudio.dll
        # echo "./PixelEngine/lib/FMOD_studio/lib/x86/fmodstudioL.dll -> ./PixelEngine/lib/bin/x86/fmodstudioL.dll"
        # cp ./PixelEngine/lib/FMOD_studio/lib/x86/fmodstudioL.dll ./PixelEngine/lib/bin/x86/fmodstudioL.dll
        
        # echo "./PixelEngine/lib/FMOD_studio/lib/x64/fmodstudio.dll -> ./PixelEngine/lib/bin/x64/fmodstudio.dll"
        # cp ./PixelEngine/lib/FMOD_studio/lib/x64/fmodstudio.dll ./PixelEngine/lib/bin/x64/fmodstudio.dll
        # echo "./PixelEngine/lib/FMOD_studio/lib/x64/fmodstudioL.dll -> ./PixelEngine/lib/bin/x64/fmodstudioL.dll"
        # cp ./PixelEngine/lib/FMOD_studio/lib/x64/fmodstudioL.dll ./PixelEngine/lib/bin/x64/fmodstudioL.dll
        
        # echo "Done."
        
        # echo "Deleting ./api/ ..."
        # rm -rf ./api
        # echo "Done."
    # else
        # echo "./api/ not found, exiting..."
        # exit 1; 
    # fi
# fi



echo ""
echo "== Setup complete! =="
echo "Continue to the next step"
