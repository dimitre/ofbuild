#include <iostream>
#include <filesystem>
#include <vector>
#include <fmt/format.h>
// #include <fmt/core.h>
#include <yaml-cpp/yaml.h>

namespace fs = std::filesystem;
using std::cout;
using std::endl;
using std::string;
using std::vector;

struct builder {
public:
std::string signx = R"(________                       ___________                                                __            
\_____  \ ______   ____   ____ \_   _____/___________    _____   ______  _  _____________|  | __  ______
 /   |   \\____ \_/ __ \ /    \ |    __) \_  __ \__  \  /     \_/ __ \ \/ \/ /  _ \_  __ \  |/ / /  ___/
/    |    \  |_> >  ___/|   |  \|     \   |  | \// __ \|  Y Y  \  ___/\     (  <_> )  | \/    <  \___ \ 
\_______  /   __/ \___  >___|  /\___  /   |__|  (____  /__|_|  /\___  >\/\_/ \____/|__|  |__|_ \/____  >
        \/|__|        \/     \/     \/               \/      \/     \/                        \/     \/)";

std::string sign = R"(
██████ █████ ░░░  ▒  ▒ ▒ ▓    ███
█    █ █     ░  ░ ▒  ▒ ▒ ▓    █  █
█    █ ████  ░░░  ▒  ▒ ▒ ▓    █  █
█    █ █     ░  ░ ▒  ▒ ▒ ▓    █  █
██████ █     ░░░  ▒▒▒▒ ▒ ▓▓▓▓ ███
)";

	YAML::Node config;
	fs::path ofPath;
	// fs::path buildPath = "builder";
	fs::path buildPath = "local_addons";
	// FIXME: maybe unneded
	vector<fs::path> addonsPath;
	vector<string> addonsStr;

	vector<string> nodeToStrings(const string & index) {
		auto items = config[index];
		vector <string> out;
		for (std::size_t i=0;i<items.size();i++) {
			out.emplace_back(items[i].as<string>());
		}
		return out;
	}

	string colorText(const string & s, int color = 32) {
		string c = std::to_string(color);
		return "\033[1;"+c+"m" + s + "\033[0m";
	}
	void msg(const string & s, int color = 32) {
		cout << colorText(s,color) << endl;
	}

	void bold(const string & s) {
		cout << "\e[1m" + s + "\e[0m" << endl;
	}

	void checkAddon(const string & f) {

		if (f.substr(0,3) == "git") {
			// cout << "git addon ----" << f << endl;
			if (!fs::exists(buildPath)) {
				fs::create_directory(buildPath);
			}
			// string command = "cd "+ buildPath.string() + "; git clone --quiet --single-branch --config \"advice.detachedHead=false\" " + f + " --depth 1 " ;
			string command = "cd "+ buildPath.string() + "; git clone --single-branch --config \"advice.detachedHead=false\" " + f + " --depth 1 " ;
			cout << command << endl;
			bool ok = system(command.c_str());
			// cout << ok << endl;
			divider();
		} else {
			// cout << "core addon ----" << f << endl;
			// cout << "::: " << f << endl;
			addonsStr.emplace_back(f);

			// fixme: remove
			auto addonPath = ofPath / "addons" / f;
			if (fs::exists(addonPath)) {
				addonsPath.emplace_back(addonPath);
			}
		}
	}

	void divider() {
		cout << "--------------------------------------------------------------------------------------------------------" << endl;
	}

	void load() {

		divider();
		msg(sign,32);
		cout << "Build System for OpenFrameworks v.0.01" << endl;
		msg("http://dmtr.org/", 34);
		
		cout << "current path " << std::filesystem::current_path() << endl;
		cout << endl;

		fs::path configFile = "of.yml";
		if (!fs::exists(configFile)) {
			msg("missing of.yml file " + ofPath.string(), 31);
			divider();
			std::exit(0);
		}
		config = YAML::LoadFile(configFile);
		auto ofPathYML = config["ofpath"];
		ofPath = ofPathYML.as<string>();
		if (!fs::exists(ofPath)) {
			msg("ofpath does not exist " + ofPath.string(), 31);
			divider();
			std::exit(0);
		}

		fs::path pgPath = ofPath / "apps/projectGenerator/commandLine/bin/projectGenerator.app/Contents/MacOS/projectGenerator";
		if (!fs::exists(pgPath)) {
			msg("projectGenerator path does not exist \n" + pgPath.string(), 31);
			divider();
			std::exit(0);
		}

		// for (auto & s : nodeToStrings("platforms")) {
		// 	cout << s << endl;
		// }
		// for (auto & s : nodeToStrings("templates")) {
		// 	cout << s << endl;
		// }
		// YAML::Node config = YAML::LoadFile("of.yml");
		for (auto & a : nodeToStrings("addons")) {
			checkAddon(a);
		}

		bold("addons");
		for (auto & a : addonsStr) {
			msg(a);
		}
 		// auto addons = config["addons"];
		// for (std::size_t i=0;i<addons.size();i++) {
		// 	if (const YAML::Node *pName = addons[i].FindValue("git")) {
		// 		std::string name = (*pName).as<string>();
		// 		cout << "GIT VALUE " << name << endl;
		// 	} else {
		// 		checkAddon(addons[i].as<string>());
		// 	}
		// }
		divider();

		vector<string> commands;
		commands.emplace_back(pgPath.string());
		commands.emplace_back("-o" + ofPath.string());

		if (nodeToStrings("platforms").size()) {
			string s = "-p\"" + 
			fmt::format("{}",fmt::join(nodeToStrings("platforms"),",")) 
			+ "\"";
			commands.emplace_back(s); 
		}

		if (nodeToStrings("templates").size()) {
			string s = "-t\"" + 
			fmt::format("{}",fmt::join(nodeToStrings("templates"),",")) 
			+ "\"";
			commands.emplace_back(s); 
		}

		string addonString = "";
		if (addonsStr.size()) {
			addonString += fmt::format("{}",fmt::join(addonsStr,","));
		}

		vector<string> gitAddons;
		if (fs::exists(buildPath)) {
			for (const auto & a : fs::directory_iterator(buildPath)) {
				gitAddons.emplace_back(a.path().string());
			}
			if (gitAddons.size()) {
				if (addonString != "") {
					addonString += ",";
				}
				addonString += fmt::format("{}",fmt::join(gitAddons,",")) ;
			}
		}

		if (addonString != "") {
			string s = "-a\"" + 
			addonString
			+ "\"";
			commands.emplace_back(s); 
		}
		commands.emplace_back(".");

//xaxa
// list all cloned addons
	// bold("cloned addons");


		bold("commands");
		for (auto & c : commands) {
			msg(c);
		}
		


		string command = fmt::format("{}",fmt::join(commands," "));
		msg (command, 34);

		bool ok = system(command.c_str());
		if (ok != 0) {
			msg ("error");
		}

// pg  -o"/Volumes/tool/ofw" -a"ofxARKit" -p"ios,linuxarmv7l,osx" -s", /Volumes/tool/ofw/apps/AreiaApps/_AREIA/AREIA/src" -t"gitignore" "/Volumes/tool/ofw/apps/myApps/mySketch"



	}
} build;


//          foreground background
// black        30         40
// red          31         41
// green        32         42
// yellow       33         43
// blue         34         44
// magenta      35         45
// cyan         36         46
// white        37         47

int main(const int argc, const char* argv[]) {
	build.load();
	return 0;
}

