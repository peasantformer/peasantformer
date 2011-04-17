#include <Peasantformer/Modules/Modules.h>

int Modules::add_dir(std::string dir) {
	this->modules_dirs.push_back(dir);
	return 0;
}

int Modules::load_modules_in_dirs(std::string dir) {
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
			
			if (this->load_module(fname, this->renders, this->objects) == 0) {
//				printf("%s module loaded\n",fname.c_str());
			}
			
		} else if (S_ISDIR(buf.st_mode)) {
			this->load_modules_in_dirs(fname);
		}
	}
	closedir(directory);
	return 0;
}
