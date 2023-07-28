# Chappie
## Installation & Setup
On the MacOS I've created an automated setup script. This script hasn't been tested on Linux or Window operating systems. If not on a MacOS just work through each line of the *setup* file in the **Chappie** directory.

1. Download the repository
```bash
git clone https://github.com/JSpencerPittman/Chappie
```
2. Navigate into the repository and give the setup file executable permissions
```bash
cd Chappie
chmod +x setup
```
3. Run the setup executable
```bash
./setup
```

## Usage
The Chappie executable is a really a collection of different related vision programs.
The two main usages are
```bash
./Chappie IMAGE <PROGRAM> <IMAGE_PATH>
./Chappie VIDEO <PROGRAM>
```

The video will use the first available camera it finds on your computer.<br>
For the **PROGRAM** tag there are multiple options available:<br>
**DETECTION**: Detects the bowl, white patches and color pad measurement device and draws boxes on them.<br>
**LEVEL**: Detects the bowl and rotates the image to make the bowl level.<br>
**NONE**: Just displays the image or live video stream itself.<br>
**RED**: what the camera sees when looking for the bowl.<br>
**WHITE**: what the camera sees when looking for the white patches.<br>
**GREEN**: what the camera sees when looking for the color pad measuring device.<br>
