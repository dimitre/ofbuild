#define VERSION "Build System for OpenFrameworks v0.0.9"

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <yaml-cpp/yaml.h>

#include <iostream>
#if __has_include(<filesystem>)
	#include <filesystem>
#else
	#include <experimental/filesystem>
#endif	
#include <vector>
#include <regex>
#include <fstream>

using std::cout;
using std::endl;
using std::string;
using std::vector;

// namespace fs = std::filesystem;
namespace fs = std::__fs::filesystem;


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
	// fs::path buildPath = "local_addons";
	fs::path buildPath = "build_addons";
	// FIXME: maybe unneded
	vector<fs::path> addonsPath;
	vector<string> addonsStr;
	vector<string> sourcesStr;

	void import() {
		// msg("not yet implemented", 32);
		// exit(0);
		if (fs::exists("of.yml")) {
			msg("of.yml already present", 32);
		} else {
			std::ifstream file("addons.make");
			YAML::Node node;
			node["ofpath"] = "../../..";
			if (file.is_open()) {
				std::string line;
				while (std::getline(file, line)) {
					// msg(line, 33);
					node["addons"].push_back(line);
				}
			}
			file.close();
			// std::string saida = node.as<std::string>();
			cout << node << endl;

			std::ofstream ofYml("of.yml");
    		ofYml << node;
		    ofYml.close();
			cout << endl;
			msg("ok, of.yml created from addons.make", 32);
		}
		std::exit(0);
	}

	void clear() {
		cout << "clear all " << buildPath << endl;
		fs::remove_all(buildPath);
	}

	void make() {
		system("make -j");
	}

	void makeRun() {
		system("make -j; make RunRelease");
	}

	vector<string> nodeToStrings(const string & index) {
		vector <string> out;
		if (config[index]) {
			auto items = config[index];
			for (std::size_t i=0;i<items.size();i++) {
				out.emplace_back(items[i].as<string>());
			}
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
	void title(const string & s, int color = 32) {
		cout << colorText("░░░▒▓█ "+s,color) << endl;
	}

	void bold(const string & s) {
		cout << "\x1B[1m" + s + "\x1B[0m" << endl;
	}

	void checkAddon(const string & f) {

		if (fs::exists(ofPath / "addons" / f) || fs::exists(f)) {
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
			// std::regex findRepository {"([^/]+)?(?:.git)?$"};
			std::regex findRepository {"^(?:https?:\\/\\/|git@)(?:[^:/]+)[:/]{1}(?:.+)/([^./]+)(?:.git)?$"};
			
			std::smatch varMatch;
			if(std::regex_search(f, varMatch, findRepository)) {
				std::string repo = varMatch[1].str();
				// for (auto & v : varMatch) {
				// 	cout << v.str() << endl;
				// }
				// cout << "REGEX " << f << " : " << p << endl;
				fs::path repoPath = buildPath / repo;
				if (fs::exists(repoPath)) {
					cout << "💾 " << repoPath <<  endl;
					// std::string command = "cd" + repoPath.string() + "; git pull";
					// bool ok = system(command.c_str());
				} else {
					// string command = "cd "+ buildPath.string() + "; git clone --single-branch --config \"advice.detachedHead=false\" " + f + " --depth 1 " ;
					string command = "cd "+ buildPath.string() + "; git clone " + f ; //+ " --single-branch --config \"advice.detachedHead=false\" --depth 1 " ;
					cout << ">>>> " << endl;
					cout << command << endl;
					bool ok = system(command.c_str());
				}
				// cout << repo << endl;
				// cout << f << endl;
				// cout << varMatch[0].str() << endl;
				// for (auto & v : varMatch) {
				// 	cout << v.str() << endl;
				// }
			} else {
				msg("OFBUILD: invalid git address regex : " + f, 31);
					string command = "cd "+ buildPath.string() + "; git clone " + f ; //+ " --single-branch --config \"advice.detachedHead=false\" --depth 1 " ;
					cout << ">>>> " << endl;
					cout << command << endl;
					bool ok = system(command.c_str());

			}
		} 
		divider();
	}

	void divider() {
		cout << "--------------------------------------------------------------------------------------------------------" << endl;
	}

	void billboard() {
		divider();
		msg(sign,32);
		cout << VERSION << endl;
		msg("Dimitre Lima http://dmtr.org/", 34);
		cout << endl;
	}

	void load() {

		fs::path configFile = "of.yml";
		bool hasConfig = false;

		// check if of.yml file is present.
		if (!fs::exists(configFile)) {
			msg("missing of.yml file " + ofPath.string(), 31);
			divider();
			// std::exit(0);
			ofPath = "../../..";
		} else {
			hasConfig = true;
			config = YAML::LoadFile(configFile);
			if (config["ofpath"]) {  // use ofpath only if the key exists.
				auto ofPathYML = config["ofpath"];
				ofPath = ofPathYML.as<string>();
			} else {
				ofPath = "../../..";
			}

			
			if (!fs::exists(ofPath)) {
				msg("ofpath does not exist " + ofPath.string(), 31);
				divider();
				std::exit(0);
			}
		}

		fs::path pgPath;

		if (config["pgPath"]) {
			pgPath = config["pgPath"].as<string>();
		} else {
			vector <fs::path> pgPaths { 
				ofPath / "apps/pgd/commandLine/bin/projectGenerator",
				ofPath / "apps/pgd/commandLine/bin/projectGenerator.app/Contents/MacOS/projectGenerator",
				ofPath / "apps/pgd/commandLine/bin/commandLine.app/Contents/MacOS/commandLine",
				ofPath / "apps/pgd/commandLine/bin/commandLineDebug.app/Contents/MacOS/commandLine",
				ofPath / "apps/projectGenerator/commandLine/bin/projectGenerator",
				ofPath / "apps/projectGenerator/commandLine/bin/projectGenerator.app/Contents/MacOS/projectGenerator",
				ofPath / "apps/projectGenerator/commandLine/bin/commandLine.app/Contents/MacOS/projectGenerator",
				ofPath / "apps/projectGenerator/commandLine/bin/commandLineDebug.app/Contents/MacOS/projectGenerator",
			};

			// try to find PG in different paths, choose the first found.
			for (auto & p : pgPaths) {
				if (fs::exists(p)) {
					pgPath = p;
					// cout << "found pgpath = " << pgPath << endl;
					break;
				} else {
				}
			}

			// exit if pg Path doesn't exist.
			if (pgPath.empty()) {
				std::string message = "projectGenerator path does not exist \n";
				for (auto & p : pgPaths) {
					message += p.string() + "\n";
				}
				msg(message, 31);
				divider();
				std::exit(0);
			}
		}

		if (!hasConfig) {
			// invoke pg.
			string command = pgPath.string() + " -o../../.. . ";
			cout << command << endl;
			system(command.c_str());
			msg("no of.yml present. use `ofbuild import` to create one from addons.make", 32);
			std::exit(0);
		}

		if (config["name"]) {
			auto nameYML = config["name"];
			msg(nameYML.as<string>(), 31);
		}
		msg("current path		" + fs::current_path().string(), 32);

		// fs::path pgPath = ofPath / "apps/projectGenerator/commandLine/bin/projectGenerator.app/Contents/MacOS/projectGenerator";
		// std::vector<fs::path> pgPaths = 


		msg("OF PG path		" + pgPath.string(), 32);
		cout << endl;
		// bold("Addons");
		title("Addons ");
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
		title("Platforms ");
		for (auto & s : nodeToStrings("platforms")) {
			cout << s << endl;
		}
		cout << endl;
		title("Templates ");
		for (auto & s : nodeToStrings("templates")) {
			cout << s << endl;
		}

		title("Additional Source Directories ");
		for (auto & a : nodeToStrings("sources")) {
			if (a != "") {
				sourcesStr.emplace_back(a);
				cout << a << endl;
			}
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
		commands.emplace_back("-o\"" + ofPath.string() + "\"");

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

		if (nodeToStrings("frameworks").size()) {
			string s = "-f\"" + 
			fmt::format("{}",fmt::join(nodeToStrings("frameworks"),",")) 
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

		if (sourcesStr.size()) {
			string s = "-s\"" + 
			fmt::format("{}",fmt::join(sourcesStr,","))
			+ "\"";
			commands.emplace_back(s); 
		}

		if (!config["projectPath"]) {
			commands.emplace_back(".");
		} else {
			commands.emplace_back(config["projectPath"].as<string>());
		}

		//xaxa
		// list all cloned addons
		// bold("cloned addons");

		// msg("Commands ", 32);
		// for (auto & c : commands) {
		// 	cout << c << endl;
		// 	// msg(c);
		// }

		title("Invoking PG ");
		string command = fmt::format("{}",fmt::join(commands," "));
		msg (command, 34);


		bool ok = system(command.c_str());
		if (ok != 0) {
			msg ("error");
		} else {
			title("PG Done ");
		}

		// bool ok2 = system("open *.xcodeproj");
	}

	void open() {
		std::string command = "open *.xcodeproj";
		cout << command << endl;
		system(command.c_str());
	}
} build;

int main(const int argc, const char* argv[]) {
	build.billboard();

	if (argc == 1) {
		build.load();
	}
	// change here to any number of parameters.
	
	if (argc == 2) {
		string param = argv[1];

		if (param == "import") {
			build.import();
			build.load();
		}
		else if (param == "open") {
			build.load();
			cout << "end load" << endl;
			build.open();
			cout << "end open" << endl;
		}
		else if (param == "xcodebuild") {
			build.load();
			system("xcodebuild");
		}
		else if (param == "edit") {
			system("open of.yml");
		}
		else if (param == "make") {
			build.load();
			build.make();
		}
		else if (param == "makerun") {
			build.load();
			build.makeRun();
		}
		else if (param == "clear") {
			build.clear();
		}
	}
	build.divider();
	return 0;
}
