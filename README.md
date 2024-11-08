```
████ ███ ░░░  ▒  ▒ ▒ ▓    ███  TM
█  █ █   ░  ░ ▒  ▒ ▒ ▓    █  █  
█  █ ███ ░░░  ▒  ▒ ▒ ▓    █  █  
█  █ █   ░  ░ ▒  ▒ ▒ ▓    █  █  
████ █   ░░░  ▒▒▒▒ ▒ ▓▓▓▓ ███  
```  
# OFBUILD (macOS only today)
Project to build reproduceable templates for openFrameworks projects using recipes.  
Recipes uses the YAML format, similar to github actions  

# Install
```
brew tap dimitre/ofbuild
brew install ofbuild
```

in your project folder create a file called ```of.yml``` with the settings similar to ```of.yml``` in this repo.

with the command ```ofbuild``` you generate the project with the configurations specified in of.yml  
other options  
```ofbuild``` : clone local addons, generate the project
```ofbuild -h``` or ```ofbuild --help``` : help  
```ofbuild clean```  
```ofbuild edit``` : opens of.yml in your editor of choice  
```ofbuild open``` : same as ofbuild then open created project in xcode (maybe rename to ofbuild xcode?)  
```ofbuild make``` : same as ofbuild then compiles using make  
```ofbuild makerun``` : same as ofbuild then compiles and run  
```ofbuild xcodebuild``` : build template and run xcodebuild  
  
Naming, parameters, api will potentially change in the next few weeks.  
Ideas are welcome about everything.  

# Important
it works well with the latest projectGenerator from github. updated 15 apr 2023.  
it will try to open the default folders relative to your ofpath, specified in ```of.yml```  
you can build projectGenerator command line using
```
cd $ofw/apps/projectGenerator/commandLine
make -j
```

# of.yml sample
```yaml
name: AeroFractal

ofpath: /Volumes/tool/ofw

addons:
- ofxAssimpModelLoader
- ofxKinect
- ofxGui
- ofxOpenCV
- ofxNetwork
- ofxSvg
- ofxXmlSettings
- git@github.com:danomatika/ofxMidi.git
- git@github.com:dimitre/ofxMicroUI.git
- git@github.com:dimitre/ofxMicroUIMidiController.git
- git@github.com:astellato/ofxSyphon.git

additionalSourceFolders:
  # ../../src
  # /Volumes/tool/ofw/additional

platforms: ["osx", "vs"]
templates: ["vscode"]
```

# random notes:
https://textpaint.net

```tsv
	foreground	background
black	30	40
red	31	41
green	32	42
yellow	33	43
blue	34	44
magenta	35	45
cyan	36	46
white	37	47
```
