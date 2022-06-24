#include <boost/python.hpp>

#include "../Renderer/DSCS/Renderer.hpp"

using namespace boost::python;
using namespace Rendering::DSCS;

class pyRenderer
{
private:
	Renderer renderer;
public:
	void initRenderer()
	{
		renderer.initRenderer();
	}

	void loadModel(std::string filepath)
	{
		renderer.loadModel(filepath);
	}

	void recalculateGlobalUniforms()
	{
		renderer.recalculateGlobalUniforms();
	}

	void refreshRenderSettings()
	{
		renderer.refreshRenderSettings();
	}

	void render()
	{
		renderer.render();
	}

	void setCameraPosition(list pos)
	{
		auto x = extract<float>(pos[0]);
		auto y = extract<float>(pos[1]);
		auto z = extract<float>(pos[2]);
		renderer.camera.setPosition({x,y,z});
	}
};

BOOST_PYTHON_MODULE(pyDSCSRenderer)
{
	class_<pyRenderer, boost::noncopyable>("DSCSRenderer")
		.def("initRenderer",              &pyRenderer::initRenderer)
		.def("refreshRenderSettings"    , &pyRenderer::refreshRenderSettings)
		.def("recalculateGlobalUniforms", &pyRenderer::recalculateGlobalUniforms)
		.def("render",                    &pyRenderer::render)
		.def("loadModel",                 &pyRenderer::loadModel, arg("filepath"))
		.def("setCameraPosition",         &pyRenderer::setCameraPosition, arg("pos"));
}
