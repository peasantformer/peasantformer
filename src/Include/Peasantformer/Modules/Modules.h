#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <vector>
#include <string>
#include <map>

template <class T>
class Modules {
	private:
		std::vector<std::string> modules_dirs;
		std::map<std::string,T> modules;
	public:
		int add_dir(std::string dir); 
		int load_modules_in_dirs(std::string dir = "");
		int load_module(std::string path);
};

template<class T>
int Modules<T>::add_dir(std::string dir) {
	this->modules_dirs.push_back(dir);
	return 0;
}

template<class T>
int Modules<T>::load_modules_in_dirs(std::string dir) {
	if (dir == "") {
		for(std::vector<std::string>::iterator it = this->modules_dirs.begin(); it != this->modules_dirs.end(); it++) {
			if (*it != "") {
				this->load_modules_in_dirs(*it);
			}
		}
		return 0;
	}	
	DIR *directory = NULL;
	struct dirent *derp = NULL;
	
	directory = opendir(dir.c_str());
	if (directory == NULL) {
		perror(dir.c_str());
		return -1;
	}
	std::string fname;
	while ((derp = readdir(directory)) != NULL) {
		if (derp->d_name == std::string(".")) continue;
		if (derp->d_name == std::string("..")) continue;
		fname = dir + "/" + derp->d_name;
		struct stat buf;
		stat(fname.c_str(),&buf);
		if (S_ISREG(buf.st_mode))	 {
			if (this->load_module(fname) == 0) {
				printf("%s module loaded\n",fname.c_str());
			}
		} else if (S_ISDIR(buf.st_mode)) {
			this->load_modules_in_dirs(fname);
		}
	}
	return 0;
}

template<class T>
int Modules<T>::load_module(std::string path) {
#ifdef _WIN32


#else



#endif
	return 0;
}
