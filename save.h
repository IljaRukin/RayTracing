#pragma once

float maximum_value(color image[height][width])
{
	//rescale image
	float current_max; float max_value = -1e+10;
	for (unsigned row = 0; row < height; row++)
	{
		for (unsigned column = 0; column < width; column++)
		{
			current_max = image[row][column].maximum();
			if (max_value < current_max) { max_value = current_max; }
		}
	}
	return max_value;
}

float minimum_value(color image[height][width])
{
	//rescale image
	float current_min; float min_value = 1e+10;
	for (unsigned row = 0; row < height; row++)
	{
		for (unsigned column = 0; column < width; column++)
		{
			current_min = image[row][column].minimum();
			if (min_value > current_min) { min_value = current_min; }
		}
	}
	return min_value;
}

void save_img(color image[height][width], float offset, float scaling, string name)
{
    //write to file
    ofstream myfile;
    myfile.open(name);
    myfile << "P3\n" << width << " " << height << "\n255\n";
    for (unsigned row = 0; row < height; row++)
	//for (int row = height - 1; row >= 0; row--)
    {
        for (unsigned column = 0; column < width; column++)
        {
            ((image[row][column]+offset)*scaling).output_uint8(&myfile);
        }
    }
    myfile.close();
}
