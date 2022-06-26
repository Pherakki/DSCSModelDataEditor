#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>

#include "../Renderer/DSCS/Renderer.hpp"

using namespace boost::python;
using namespace Rendering::DSCS;

class pyRenderer
{
private:
	Renderer renderer;
public:
	void initRenderer             () { renderer.initRenderer(); }
	void recalculateGlobalUniforms() { renderer.recalculateGlobalUniforms(); }
	void refreshRenderSettings    () { renderer.refreshRenderSettings(); }
	void render                   () { renderer.render(); }

	void advanceClock(float increment)                { renderer.advTime(increment); }
	int  loadModel(std::string filepath)              { renderer.loadModel(filepath); return renderer.models.size() - 1; }
	void loadAnim(int model_id, std::string filepath) { renderer.loadAnim(renderer.models[model_id], filepath); }
	void setAspectRatio(int width, int height)        { renderer.aspect_ratio = (float)width / (float)height; }
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
		.def("advanceClock",              &pyRenderer::advanceClock, arg("increment"))
		.def("loadModel",                 &pyRenderer::loadModel, arg("filepath"))
		.def("setCameraPosition",         &pyRenderer::setCameraPosition, arg("pos"))
		.def("loadAnim",                  &pyRenderer::loadAnim,  arg("model_id"), arg("filepath"))
		.def("setAspectRatio",            &pyRenderer::setAspectRatio, arg("width"), arg("height"));
}

