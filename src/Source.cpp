#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>

const bool enable_minimization = true;
const bool enable_save_in_file = true;
const bool enable_print_execution_time = true;

struct point
{
	long long id;
	double x;
	double y;
	int color;
};

double euclidean_distance(const point& p1, const point& p2)
{
	return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

point find_bottom_point(const std::vector<point>& points)
{
	point result = points[0];
	for (long long i = 1; i < points.size(); ++i)
	{
		if (points[i].y < result.y)
		{
			result = points[i];
		}
	}
	return result;
}

double calculate_angle(const point& bottom_point, const point& cur_point)
{
	double x_diff = cur_point.x - bottom_point.x;
	double y_diff = cur_point.y - bottom_point.y;
	return x_diff / std::sqrt(x_diff * x_diff + y_diff * y_diff);
}

std::vector<std::pair<double, point>> calculate_angles(const std::vector<point>& points, const point& bottom_point)
{
	std::vector<std::pair<double, point>> result;
	for (const point& cur_point : points)
	{
		if (cur_point.id != bottom_point.id)
		{
			result.push_back(std::make_pair(calculate_angle(bottom_point, cur_point), cur_point));
		}
	}
	return std::move(result);
}

point find_pair(const std::vector<std::pair<double, point>>& angles, const point& bottom_point)
{
	long long counter = 0;
	for (long long i = 0; i < angles.size(); ++i)
	{
		if (angles[i].second.color != bottom_point.color)
		{
			if (counter == 0)
			{
				return angles[i].second;
			}
			else
			{
				++counter;
			}
		}
		else
		{
			--counter;
		}
	}
}

point find_pair_with_distances(const std::vector<std::pair<double, point>>& angles, const point& bottom_point)
{
	long long counter = 0;
	double min_distance;
	bool flag = true;
	point selected_point;
	for (long long i = 0; i < angles.size(); ++i)
	{
		if (angles[i].second.color != bottom_point.color)
		{
			if (counter == 0)
			{
				if (flag)
				{
					flag = false;
					min_distance = euclidean_distance(bottom_point, angles[i].second);
					selected_point = angles[i].second;
				}
				else
				{
					double cur_distance = euclidean_distance(bottom_point, angles[i].second);
					if (cur_distance < min_distance)
					{
						min_distance = cur_distance;
						selected_point = angles[i].second;
					}
				}
			}
			++counter;
		}
		else
		{
			--counter;
		}
	}
	return selected_point;
}

void fun(const std::vector<point>& points, std::vector<std::pair<long long, long long>>& pairs, double& average_length)
{
	if (points.size() == 2)
	{
		pairs.push_back(std::make_pair(points[0].id, points[1].id));
		average_length += euclidean_distance(points[0], points[1]);
		return;
	}
	point bottom_point = find_bottom_point(points);
	std::vector<std::pair<double, point>> angles = calculate_angles(points, bottom_point);
	std::sort(angles.begin(), angles.end(), [](const std::pair<double, point>& left, const std::pair<double, point>& right) {
		return left.first < right.first;
	});
	point second_point;
	if (enable_minimization)
	{
		second_point = find_pair_with_distances(angles, bottom_point);
	}
	else
	{
		second_point = find_pair(angles, bottom_point);
	}
	pairs.push_back(std::make_pair(bottom_point.id, second_point.id));
	average_length += euclidean_distance(bottom_point, second_point);
	std::vector<point> points_left, points_right;
	bool found_flag = false;
	for (long long i = 0; i < angles.size(); ++i)
	{
		if (angles[i].second.id == second_point.id)
		{
			found_flag = true;
		}
		else
		{
			if (!found_flag)
			{
				points_left.push_back(angles[i].second);
			}
			else
			{
				points_right.push_back(angles[i].second);
			}
		}
	}
	if (points_left.size() > 0)
	{
		fun(points_left, pairs, average_length);
	}
	if (points_right.size() > 0)
	{
		fun(points_right, pairs, average_length);
	}
}

int main()
{
	std::string file_name;
	std::cout << "Input path to data file..." << std::endl;
	std::cin >> file_name;
	std::ifstream fin(file_name);
	long long n;
	fin >> n;
	if (n % 2 == 1)
	{
		std::cout << "N should be even" << std::endl;
		return 0;
	}
	std::vector<point> points(n);
	for (long long i = 0; i < n; ++i)
	{
		fin >> points[i].id >> points[i].x >> points[i].y >> points[i].color;
	}
	fin.close();
	auto t1 = std::chrono::high_resolution_clock::now();
	std::vector<std::pair<long long, long long>> pairs;
	double average_length = 0.0;
	fun(points, pairs, average_length);
	average_length /= n / 2;
	auto t2 = std::chrono::high_resolution_clock::now();
	std::cout << n / 2 << std::endl;
	std::cout << average_length << std::endl;
	for (const std::pair<long long, long long>& p : pairs)
	{
		std::cout << p.first << ' ' << p.second << std::endl;
	}
	if (enable_print_execution_time)
	{
		std::chrono::duration<double, std::milli> ms_double = t2 - t1;
		std::cout << "Execution time = ";
		std::cout << ms_double.count() << "ms" << std::endl;
	}
	if (enable_save_in_file)
	{
		std::ofstream fout("result.txt");
		fout << n / 2 << std::endl;
		fout << average_length << std::endl;
		for (const std::pair<long long, long long>& p : pairs)
		{
			fout << p.first << ' ' << p.second << std::endl;
		}
		fout.close();
	}
	return 0;
}
