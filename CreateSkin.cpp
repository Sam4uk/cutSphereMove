#include "CreateSkin.hpp"
#include <fstream>

double clamp(double val, double min, double max)
{
	return (std::max(min, std::min(val, max)));
}

bool InSphere  (const cadcam::mwTPoint3d<double> &test_point,
				const cadcam::mwTPoint3d<double> &sphere_center,
				const double &sphereRad)
{
	return (test_point - sphere_center) * (test_point - sphere_center) <= sphereRad * sphereRad;
}

void 	CreateSkin( const cadcam::mwTPoint3d<double> refPoint,
					const unsigned long nx, const unsigned long ny,
					const unsigned long nz, const double sphereRad,
					mwDiscreteFunction &func, const double deltaT,
					const double delta, const std::string &skinFileName )
{
	std::vector<cadcam::mwTVector3d<double>> points;
	for (auto cur{func.GetBeginParameter()}; cur <= func.GetEndParameter(); cur += deltaT)
		points.push_back(func.Evaluate(cur) - refPoint);

	auto pos_count = points.size();

	cadcam::mwTVector3d<unsigned long> point_min[pos_count];
	cadcam::mwTVector3d<unsigned long> point_max[pos_count];
	for (unsigned long i{0}; i < pos_count; i++)
	{
		point_min[i].x(static_cast<unsigned long>(clamp(ceil(points[i].x() - sphereRad), 0.0, static_cast<double>(nx))));
		point_min[i].y(static_cast<unsigned long>(clamp(ceil(points[i].y() - sphereRad), 0.0, static_cast<double>(ny))));
		point_min[i].z(static_cast<unsigned long>(clamp(ceil(points[i].z() - sphereRad), 0.0, static_cast<double>(nz))));

		point_max[i].x(static_cast<unsigned long>(clamp(ceil(points[i].x() + sphereRad), 0.0, static_cast<double>(nx))));
		point_max[i].y(static_cast<unsigned long>(clamp(ceil(points[i].y() + sphereRad), 0.0, static_cast<double>(ny))));
		point_max[i].z(static_cast<unsigned long>(clamp(ceil(points[i].z() + sphereRad), 0.0, static_cast<double>(nz))));
	}

	std::vector < std::vector <unsigned long> > skin(nx, std::vector<unsigned long>(ny, nz));

	for (unsigned long n = 0; n < pos_count; n++)
	{
		for (auto x_dev{point_min[n].x()}; x_dev < point_max[n].x(); x_dev++)
		{
			for (auto y_dev{point_min[n].y()}; y_dev < point_max[n].y(); y_dev++)
			{
				for (auto z_dev{point_min[n].z()}; z_dev < point_max[n].z(); z_dev++)
				{
					if (skin[x_dev][y_dev] < z_dev) break;
					cadcam::mwTVector3d<double> testpoint(x_dev * delta, y_dev * delta, z_dev * delta);
					if (InSphere(testpoint, points[n], sphereRad))
						skin[x_dev][y_dev] = z_dev - 1;
				}
			}
		}
	}

	{
		std::ofstream SkinFile(skinFileName);
		for (auto X{0}; X < nx; X++)
			for (auto Y{0}; Y < ny; Y++)
				SkinFile << X * delta << ' '
						 << Y * delta << ' '
						 << skin[X][Y] * delta << std::endl;
		SkinFile.close();

	}
}
