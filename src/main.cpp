
#include <fmt/format.h>
#include <yaml-cpp/yaml.h>

#include <iostream>
#include <filesystem>
#include <vector>
#include <regex>

using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace fs = std::filesystem;

struct builder {
public:
std::string signx = R"(________                       ___________                                                __            
\_____  \ ______   ____   ____ \_   _____/___________    _____   ______  _  _____________|  | __  ______
 /   |   \\____ \_/ __ \ /    \ |    __) \_  __ \__  \  /     \_/ __ \ \/ \/ /  _ \_  __ \  |/ / /  ___/
/    |    \  |_> >  ___/|   |  \|     \   |  | \// __ \|  Y Y  \  ___/\     (  <_> )  | \/    <  \___ \ 
\_______  /   __/ \___  >___|  /\___  /   |__|  (____  /__|_|  /\___  >\/\_/ \____/|__|  |__|_ \/____  >
        \/|__|        \/     \/     \/               \/      \/     \/                        \/     \/)";

std::string sign = R"(
██████ █████ ░░░  ▒  ▒ ▒ ▓    ███  TM
█    █ █     ░  ░ ▒  ▒ ▒ ▓    █  █
█    █ ████  ░░░  ▒  ▒ ▒ ▓    █  █
█    █ █     ░  ░ ▒  ▒ ▒ ▓    █  █
██████ █     ░░░  ▒▒▒▒ ▒ ▓▓▓▓ ███
)";

	YAML::Node config;
	fs::path ofPath;
	// fs::path buildPath = "builder";
	// fs::path buildPath = "local_addons";
	fs::path buildPath = "ofbuild_addons";
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
		cout << "\x1B[1m" + s + "\x1B[0m" << endl;
	}

	void checkAddon(const string & f) {

		// if (f.substr(0,3) == "git") 

		if (fs::exists(ofPath / "addons" / f)) {
			// cout << "core addon ----" << f << endl;
			// cout << "::: " << f << endl;
			cout << "🏠 " << f <<  endl;

			addonsStr.emplace_back(f);

			// fixme: remove
			auto addonPath = ofPath / "addons" / f;
			if (fs::exists(addonPath)) {
				addonsPath.emplace_back(addonPath);
			}
		} 
		else 
		{
			// cout << "git addon ----" << f << endl;
			if (!fs::exists(buildPath)) {
				fs::create_directory(buildPath);
			}
			// string command = "cd "+ buildPath.string() + "; git clone --quiet --single-branch --config \"advice.detachedHead=false\" " + f + " --depth 1 " ;

			std::regex findRepository {"([^/]+)(\\.git)?$"};
			std::smatch varMatch;
			if(std::regex_search(f, varMatch, findRepository)) {
				std::string p = varMatch[0].str();
				std::string repo = p.substr(0, p.find(".git"));
				fs::path repoPath = buildPath / repo;
				if (fs::exists(repoPath)) {
					cout << "💾 " << repoPath <<  endl;
					// std::string command = "cd" + repoPath.string() + "; git pull";
					// bool ok = system(command.c_str());
				} else {
					string command = "cd "+ buildPath.string() + "; git clone --single-branch --config \"advice.detachedHead=false\" " + f + " --depth 1 " ;
					bool ok = system(command.c_str());
				}
				// cout << repo << endl;
				// cout << f << endl;
				// cout << varMatch[0].str() << endl;
				// for (auto & v : varMatch) {
				// 	cout << v.str() << endl;
				// }
			} else {
				msg("invalid git address : " + f, 31);
			}
		} 
		divider();
	}

	void divider() {
		cout << "--------------------------------------------------------------------------------------------------------" << endl;
	}

	void load() {

		divider();
		msg(sign,32);
		cout << "Build System for OpenFrameworks v.0.02" << endl;
		msg("Dimitre Lima http://dmtr.org/", 34);
		
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

		auto nameYML = config["name"];
		msg(nameYML.as<string>(), 31);
		msg("current path		" + std::filesystem::current_path().string(), 32);

		// fs::path pgPath = ofPath / "apps/projectGenerator/commandLine/bin/projectGenerator.app/Contents/MacOS/projectGenerator";
		// std::vector<fs::path> pgPaths = 
		fs::path pgPath;

		for (auto & p : {
			ofPath / "apps/pgd/commandLine/bin/projectGenerator.app/Contents/MacOS/projectGenerator",
			ofPath / "apps/pgd/commandLine/bin/projectGenerator"
		}) {
			pgPath = p;
		}

		if (pgPath.empty()) {
			msg("projectGenerator path does not exist \n" , 31);
			divider();
			std::exit(0);
		}
		msg("OF PG path		" + pgPath.string(), 32);
		cout << endl;
		// bold("Addons");
		msg("Addons ", 32);
		divider();

		for (auto & a : nodeToStrings("addons")) {
			if (a != "") {
				checkAddon(a);
			}
		}

		// bold("Addons - git clone");
		// for (auto & a : addonsStr) {
		// 	msg(a);
		// }

		auto items = config["specific_addons"];
		for(YAML::const_iterator it=items.begin();it!=items.end();++it) {
			auto item = it;
			// if (item["git"]) {
			// 	cout << item["git"].as<string>() << endl;
			// }
			// std::cout << "Playing at " << it->first.as<std::string>() << " is " << it->second.as<std::string>() << "\n";
		}

		cout << endl;
		msg("Platforms ", 32);
		for (auto & s : nodeToStrings("platforms")) {
			cout << s << endl;
		}
		cout << endl;
		msg("Templates ", 32);
		for (auto & s : nodeToStrings("templates")) {
			cout << s << endl;
		}
		cout << endl;

 		// auto addons = config["addons"];
		// for (std::size_t i=0;i<addons.size();i++) {
		// 	if (const YAML::Node *pName = addons[i].FindValue("git")) {
		// 		std::string name = (*pName).as<string>();
		// 		cout << "GIT VALUE " << name << endl;
		// 	} else {
		// 		checkAddon(addons[i].as<string>());
		// 	}
		// }

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

		// bold("commands");
		// for (auto & c : commands) {
		// 	msg(c);
		// }
		
		string command = fmt::format("{}",fmt::join(commands," "));
		msg (command, 34);

		bool ok = system(command.c_str());
		if (ok != 0) {
			msg ("error");
		}

		// bool ok2 = system("open *.xcodeproj");
	}
} build;

int main(const int argc, const char* argv[]) {
	build.load();
	return 0;
}
