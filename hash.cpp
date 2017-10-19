#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <cstring>
#include <sys/stat.h>
#include <vector>
#include <openssl/md5.h>

using namespace std;

#define FILE_BUFFER_SIZE 1024

void print_md5_sum(unsigned char* md) {
	for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
		printf("%02x", md[i]);
	}
	printf("\n");
}

void MD5_file(string filename, unsigned char* hash) {
	unsigned char buffer[FILE_BUFFER_SIZE];

	FILE* file = fopen(filename.c_str(), "rb");

	if (!file) {
		cout << "file err: " << filename << endl;
		return;
	}

	MD5_CTX mdContext;
	MD5_Init(&mdContext);
	
	int reading_bytes = 0;

	do {
		reading_bytes = fread(buffer, 1, FILE_BUFFER_SIZE, file);
		MD5_Update (&mdContext, buffer, reading_bytes);
	} while (reading_bytes > 0);
	
	MD5_Final (hash, &mdContext);

	fclose(file);
}

void write_MD5_dir(string dir_name, ofstream* results) {

	DIR* dir = opendir(dir_name.c_str());
	struct dirent* d;
	struct stat file_info;

	unsigned char hash[MD5_DIGEST_LENGTH];
	string file_fullname;
	int32_t file_fullname_len;

	while ((d = readdir(dir)) != NULL) {

		if (strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0) {

			file_fullname = dir_name + "/" + string(d->d_name);
			lstat(file_fullname.c_str(), &file_info);

			if (S_ISDIR(file_info.st_mode)) {

				write_MD5_dir(file_fullname, results);

			} else {

				// cout << 1;

				MD5_file(file_fullname, hash);
				file_fullname_len = file_fullname.length() + 1;
				results->write((char*) &file_fullname_len, sizeof(file_fullname_len));
				results->write(file_fullname.c_str(), file_fullname_len);
				results->write((const char*) hash, MD5_DIGEST_LENGTH);

				// cout << file_fullname << ": ";
				// print_md5_sum(hash);
				// cout << 2;
			}
		}
	}

	closedir(dir);
}

int main(int argc, char const *argv[])
{
	if (argc < 2) {
		cout << "Usage: " << endl << "{file with directories} [file with exceptions]" << endl;
		exit(1);
	}

	const char* directories_filename = argv[1];
	ifstream directories_file(directories_filename);
	ofstream results("MD5.hash", ios::binary);

	char dirname[PATH_MAX];

	vector<string> dirs;

	while (!directories_file.eof()) {
		directories_file.getline(dirname, PATH_MAX);
		dirs.push_back(string(dirname));
		
		if (string(dirname) != "") {
			write_MD5_dir(string(dirname), &results);
			cout << dirname << endl;
		}
	}

	results.close();

	return 0;
}