████ ███ ░░░  ▒  ▒ ▒ ▓    ███  TM
█  █ █   ░  ░ ▒  ▒ ▒ ▓    █  █  
█  █ ███ ░░░  ▒  ▒ ▒ ▓    █  █  
█  █ █   ░  ░ ▒  ▒ ▒ ▓    █  █  
████ █   ░░░  ▒▒▒▒ ▒ ▓▓▓▓ ███  
  
# OFBUILD (macOS only today)
Project to build reproduceable templates for openFrameworks projects using recipes.  
Recipes uses the YAML format, similar to github actions  

# Install
Soon we will have automatic releases on github but now you can build from source
it uses chalet build tool (https://www.chalet-work.space) to compile dependencies, so you can install from source there or install via 
```sh
brew install --cask https://raw.githubusercontent.com/dimitre/chalet_homebrew/main/chalet.rb

```

you compile by entering ofbuild directory and use one of the following commands:
```chalet build ``` compiles app and places in a folder like ./build/arm64-apple-darwin_Release/
or
```chalet bundle``` creates a zip file with the app in ./dist folder

I recommend adding ofbuild to the path so it can be invoked from your project folder.
in your project folder create a file called ```of.yml``` with the settings similar to ```of.yml``` in this repo.

with the command ```ofbuild``` you generate the project with the configurations specified in of.yml
other options
```ofbuild``` : clone local addons, generate the project
```ofbuild clean```
```ofbuild edit``` : opens of.yml in your editor of choice
```ofbuild make``` : same as ofbuild then compiles using make
```ofbuild makerun``` : same as ofbuild then compiles and run

Naming, parameters, api will potentially change in the next few weeks.
Ideas are welcome about everything.

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
