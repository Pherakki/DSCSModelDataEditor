#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>

#include "../Renderer/src/DSCS/Renderer.hpp"

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
	int  loadModel(std::string filepath)              { return renderer.loadModel(filepath); }
	void loadAnim(int model_id, std::string filepath) { renderer.loadAnim(model_id, filepath); }
	void setAspectRatio(int width, int height)        { renderer.aspect_ratio = (float)width / (float)height; }
	void setCameraPosition(float x, float y, float z) { renderer.camera.setPosition({x,y,z}); }
	void setCameraTarget(float x, float y, float z)   { renderer.camera.setTarget({ x,y,z }); }
	void translateCamera(float x, float y)            { this->renderer.camera.translate(x, y); }
	void rotateOrbitCamera(float alt, float azi)      { this->renderer.camera.incAltAzi(alt, azi); }
	void zoomCamera(float distance)                   { this->renderer.camera.mulRadius(distance); }
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
		.def("loadAnim",                  &pyRenderer::loadAnim, arg("model_id"), arg("filepath"))
		.def("setAspectRatio",            &pyRenderer::setAspectRatio, arg("width"), arg("height"))
		.def("setCameraPosition",         &pyRenderer::setCameraPosition, arg("x"), arg("y"), arg("z"))
		.def("setCameraTarget",           &pyRenderer::setCameraTarget,   arg("x"), arg("y"), arg("z"))
		.def("translateCamera",           &pyRenderer::translateCamera,   arg("x"), arg("y"))
		.def("rotateOrbitCamera",         &pyRenderer::rotateOrbitCamera, arg("alt"), arg("azi"))
		.def("zoomCamera",                &pyRenderer::zoomCamera,        arg("distance"));
}

