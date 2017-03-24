#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main()
{
	fstream myfile, outf;
	string name;
	cout << "File name: ";
	cin >> name;
	myfile.open(name, ios::in | ios::binary);
	outf.open("out.yuv", ios::out | ios::binary);

	char* data;
	int width, height;
	int posi, max = 0;
	int sum = 0;

	struct position {
		int x, y;
	};
	position pos;

	cout << "width: ";
	cin >> width;
	cout << "height: ";
	cin >> height;
	data = new char[width*height + (width*height / 2)];
	

	myfile.read(data, width*height + (width*height/2));

	for (int i = 0; i < width*height; i++) {
		if (max < data[i]) {
			max = data[i];
			posi = i;
			sum += data[i];
		}
	}

	pos.y = 0;
	pos.x = posi;
	while (pos.x > width) {
		pos.y++;
		pos.x -= width;
	}
	

	cout << "Max brightness: " << max << " in point:\n";
	cout << "x: " << pos.x << "\n";
	cout << "y: " << pos.y << "\n";
	cout << "Average: " << sum / (width*height) << "\n";

	int offset_x = 0, offset_y = 0;

	if (pos.x % 2 == 1) offset_x = 1;
	if (pos.y % 2 == 1) offset_y = 1;

	int sideLength = 28;
	char* newimg;
	newimg = new char[sideLength * sideLength + sideLength / 2 * sideLength];

	for (int i = 0; i < sideLength; i++) {
		for (int j = 0; j < sideLength; j++) {
			newimg[sideLength * i + j] = data[(posi - (sideLength / 2 - offset_y) * width - sideLength / 2) + i * width + j + (offset_x)];
		}
	}

	for (int i = 0; i < sideLength / 2; i++) {
		for (int j = 0; j < sideLength; j++) {
			newimg[sideLength * sideLength + sideLength * i + j] = data[width * height + ((width * (int)ceil(pos.y / 2.0) + (2 * (int)ceil(pos.x / 2.0))) - sideLength / 2 - sideLength / 4 * width) + i * width + j];
		}
	}

	outf.write(newimg, sideLength*sideLength + (sideLength*sideLength)/2);

	delete data;

    return 0;
}
