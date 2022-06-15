#include "ShaderFactory.hpp"

void ShaderFactory::hookUniformValueUpdateOnTextbox(size_t idx, EditorTextbox*& textbox)
{
	connect(textbox, &EditorTextbox::textChanged, this, [this, idx, &textbox]()
		{
			auto& material_resource = this->selected_objects.getEditableSelectedMaterialResource();
			auto& material = material_resource.getFactoryMaterial();
			material->setUniformValue(idx, this->sanitiseTextChanged(textbox));
			if (material_resource.isFactoryActive())
				material_resource.updateMainMaterialBuffer(idx, this->sanitiseTextChanged(textbox));
		}
	);
}

#include "moc_ShaderFactory.cpp"
