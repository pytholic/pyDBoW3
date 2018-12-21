// Author: Sudeep Pillai (spillai@csail.mit.edu)
// License: BSD
// Last modified: Sep 14, 2014

// Wrapper for most external modules
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <exception>

// Opencv includes
#include <opencv2/opencv.hpp>

// np_opencv_converter
#include "np_opencv_converter.hpp"

// DBoW3
#include "DBoW3.h"

namespace py = boost::python;

cv::Mat test_np_mat(const cv::Mat& in) {
	std::cerr << "in: " << in << std::endl;
	std::cerr << "sz: " << in.size() << std::endl;
	return in.clone();
}

cv::Mat test_with_args(const cv::Mat_<float>& in, const int& var1 = 1,
	const double& var2 = 10.0, const std::string& name = std::string("test_name")) {
	std::cerr << "in: " << in << std::endl;
	std::cerr << "sz: " << in.size() << std::endl;
	std::cerr << "Returning transpose" << std::endl;
	return in.t();
}

class GenericWrapper {
public:
	GenericWrapper(const int& _var_int = 1, const float& _var_float = 1.f,
		const double& _var_double = 1.f, const std::string& _var_string = std::string("test_string"))
		: var_int(_var_int), var_float(_var_float), var_double(_var_double), var_string(_var_string)
	{

	}

	cv::Mat process(const cv::Mat& in) {
		std::cerr << "in: " << in << std::endl;
		std::cerr << "sz: " << in.size() << std::endl;
		std::cerr << "Returning transpose" << std::endl;
		return in.t();
	}

private:
	int var_int;
	float var_float;
	double var_double;
	std::string var_string;
};

class Vocabulary
{
public:
	Vocabulary(int k = 10, int L = 5, DBoW3::WeightingType weighting = DBoW3::TF_IDF, DBoW3::ScoringType scoring = DBoW3::L1_NORM, const std::string& path = std::string()) {
	}
	~Vocabulary() {
	}
	
	void create(const  std::vector<cv::Mat>   &training_features) {
	}
	
	void clear() {
	}

	void load(const std::string& path) {
	}

	void save(const std::string& path, bool binary_compressed = true) {
	}

	DBoW3::BowVector transform(const  cv::Mat & features) {
		DBoW3::BowVector word;
		return word;
	}

private:
};

class Database
{
public:
	Database(const std::string& path = std::string()) {
	}
	~Database() {
	}

	void setVocabulary(Vocabulary& vocabulary) {
	}

	int add(const  cv::Mat & features) {
	}

	std::vector<DBoW3::Result> query(const  cv::Mat &features, int max_results = 1, int max_id = -1) {
		DBoW3::QueryResults results;
		return results;
	}

private:
};

// Wrap a few functions and classes for testing purposes
namespace fs {
	namespace python {

		BOOST_PYTHON_MODULE(pyDBoW3)
		{
			// Main types export
			fs::python::init_and_export_converters();
			py::scope scope = py::scope();

			py::enum_<DBoW3::WeightingType>("WeightingType")
				.value("TF_IDF", DBoW3::TF_IDF)
				.value("TF", DBoW3::TF)
				.value("IDF", DBoW3::IDF)
				.value("BINARY", DBoW3::BINARY);

			py::enum_<DBoW3::ScoringType>("ScoringType")
				.value("L1_NORM", DBoW3::L1_NORM)
				.value("L2_NORM", DBoW3::L2_NORM)
				.value("CHI_SQUARE", DBoW3::CHI_SQUARE)
				.value("KL", DBoW3::KL)
				.value("BHATTACHARYYA", DBoW3::BHATTACHARYYA)
				.value("DOT_PRODUCT", DBoW3::DOT_PRODUCT);

			// Class
			py::class_<Vocabulary>("Vocabulary")
				.def(py::init<py::optional<int, int, DBoW3::WeightingType, DBoW3::ScoringType, std::string> >(
				(py::arg("k") = 10, py::arg("L") = 5, py::arg("weighting") = DBoW3::TF_IDF, py::arg("scoring") = DBoW3::L1_NORM,
					py::arg("path") = std::string())))
				.def("load", &Vocabulary::load)
				.def("save", &Vocabulary::save)
				.def("create", &Vocabulary::create)
				.def("transform", &Vocabulary::transform)
				.def("clear", &Vocabulary::clear);

			py::class_<Database>("Database")
				.def(py::init<py::optional<std::string> >(py::arg("path") = std::string()))
				.def("setVocabulary", &Database::setVocabulary)
				.def("query", &Database::query);

			//py::class_< DBoW3::BowVector>("BowVector");
		}

	} // namespace fs
} // namespace python
