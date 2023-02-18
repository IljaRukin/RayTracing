#pragma once

void render_image(unsigned x_start, unsigned x_end, unsigned y_start, unsigned y_end, color image[height][width], ray rays[height][width], triangle_mesh model)
{
    unsigned row = y_start; unsigned column = x_start;
    for (row; row < y_end; row++)
    {
        //cout << "row: " << row << endl;
        for (column; column < x_end; column++)
        {
            //cout << row << " " << column << endl;
            ray r = rays[row][column];
            image[row][column].render_pixel(model, r);
        }
        column = 0;
    }
}

void render_image_multithreaded(color image[height][width], ray rays[height][width], triangle_mesh model)
{

    //launch threads
    unsigned number_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> th(number_threads);
    unsigned start_chunk = 0;
    unsigned chunk = height / number_threads;
    unsigned residue = height - chunk * number_threads;
    for (unsigned it = 0; it < number_threads; it++)
    {
        if (it < number_threads - 1)
        {
            cout << "row idx: " << start_chunk << ":" << start_chunk + chunk - 1 << endl;
            th[it] = thread(render_image, 0, width, start_chunk, start_chunk + chunk, image, rays, model);
        }
        else
        {
            cout << "row idx: " << start_chunk << ":" << start_chunk + chunk + residue - 1 << endl;
            th[it] = thread(render_image, 0, width, start_chunk, start_chunk + chunk + residue, image, rays, model);
        }
        start_chunk += chunk;
    }
    //th.back() = thread(render, x_start, x_end, y_start, y_end, model1);
    for (unsigned i = 0; i < number_threads; i++)
    {
        th[i].join();
    }
}
