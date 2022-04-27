#include <array>
#include <format>
#include <iostream>
#include <string>

#include "FragmentShader.hpp"

std::string parseOp(const Operation& op)
{
    switch (op)
    {
    case (Operation::ADD):
        return "+";
    case (Operation::SUB):
        return "-";
    case (Operation::MUL):
        return "*";
    case (Operation::DIV):
        return "/";
    default:
        throw std::exception("Bad operation");
    }
}

std::string parseMap(const TexMap& map)
{
    std::string src;
    switch (map.type)
    {
    case (MapType::DIFFUSE):
        src = "colorRGBA";
        break;
    case (MapType::NORMAL):
        src = "normalRGBA";
        break;
    case (MapType::LIGHT):
        src = "lightRGBA";
        break;
    case (MapType::OLDIFFUSE):
        src = "overlayColorRGBA";
        break;
    case (MapType::OLNORMAL):
        src = "overlayNormalRGBA";
        break;
    case (MapType::VERTEX):
        src = "vColor";
        break;
    default:
        throw std::exception("Bad maptype in transparency map.");
    }

    std::string channel;
    switch (map.channel)
    {
    case (RGBAChannel::R):
        channel = "r";
        break;
    case (RGBAChannel::G):
        channel = "g";
        break;
    case (RGBAChannel::B):
        channel = "b";
        break;
    case (RGBAChannel::A):
        channel = "a";
        break;
    default:
        throw std::exception("Bad channel in transparency map.");
    }

    return std::format("{0}.{1}", src, channel);
}

std::string parseMapWXYZ(const TexMap& map)
{
    std::string src;
    switch (map.type)
    {
    case (MapType::DIFFUSE):
        src = "colorRGBA";
        break;
    case (MapType::NORMAL):
        src = "normalRGBA";
        break;
    case (MapType::LIGHT):
        src = "lightRGBA";
        break;
    case (MapType::OLDIFFUSE):
        src = "overlayColorRGBA";
        break;
    case (MapType::OLNORMAL):
        src = "overlayNormalRGBA";
        break;
    case (MapType::VERTEX):
        src = "vColor";
        break;
    default:
        throw std::exception("Bad maptype in transparency map.");
    }

    std::string channel;
    switch (map.channel)
    {
    case (RGBAChannel::R):
        channel = "x";
        break;
    case (RGBAChannel::G):
        channel = "y";
        break;
    case (RGBAChannel::B):
        channel = "z";
        break;
    case (RGBAChannel::A):
        channel = "w";
        break;
    default:
        throw std::exception("Bad channel in transparency map.");
    }

    return std::format("{0}.{1}", src, channel);
}

std::string createHeader(const FactorySettings& settings)
{
    bool init = false;
    std::string delim = ", ";
    std::string text = "void std_PS( ";

    if (settings.dir_light_1)
    {
        if (init)
            text += delim;
        else
            init = true;
        text += "uniform half4 DirLamp01Color : Diffuse ";
        text += delim;
        text += "uniform half3 DirLamp01Dir : Direction ";
    }
    if (settings.use_ambient)
    {
        if (init)
            text += delim;
        else
            init = true;
        text += "uniform half3 AmbientColor ";
        if (settings.use_ambient_gndsky)
        {
            text += delim;
            text += "uniform half3 GroundColor ";
            text += delim;
            text += "uniform half3 SkyDir ";
        }
    }
    if (settings.texlayer_1.colorsampler.enabled)
    {
        if (init)
            text += delim;
        else
            init = true;
        text += "uniform sampler2D ColorSampler ";
    }
    if (init)
        text += delim;
    else
        init = true;
    text += "uniform half4 DiffuseColor";

    if (settings.texlayer_1.normalsampler.enabled)
    {
        text += delim;
        text += "uniform sampler2D NormalSampler ";
    }
    if (settings.use_tangents)
    {
        text += delim;
        text += "uniform half Bumpiness ";
    }
    if (settings.specular_input)
    {
        if (!settings.specular_map.enabled)
        {
            text += delim;
            text += "uniform half SpecularStrength ";
        }
        if (!settings.use_clut)
        {
            text += delim;
            text += "uniform half SpecularPower ";
        }
    }
    if (settings.use_reflection)
    {
        if (settings.refl_sphere_map)
        {
            text += delim;
            text += "uniform sampler2D EnvsSampler ";
        }
        else
        {
            text += delim;
            text += "uniform samplerCUBE EnvSampler ";
        }
        text += delim;
        text += "uniform half ReflectionStrength ";
        if (settings.use_fresnel)
        {
            text += delim;
            text += "uniform half FresnelMin ";
            text += delim;
            text += "uniform half FresnelExp : POWER ";
        }
    }
    if (settings.use_velvet)
    {
        text += delim;
        text += "uniform half3 SurfaceColor ";
        text += delim;
        text += "uniform half3 FuzzySpecColor ";
        text += delim;
        text += "uniform half3 SubColor ";
        text += delim;
        text += "uniform half RollOff ";
        text += delim;
        text += "uniform half VelvetStrength ";
    }
    if (settings.lightsampler.enabled)
    {
        text += delim;
        text += "uniform sampler2D LightSampler ";
    }
    if (settings.use_lightpower)
    {
        text += delim;
        text += "uniform half LightMapPower ";
        text += delim;
        text += "uniform half LightMapStrength ";
    }
    if (settings.texlayer_2.colorsampler.enabled)
    {
        text += delim;
        text += "uniform sampler2D OverlayColorSampler ";
    }
    if (settings.texlayer_2.normalsampler.enabled)
    {
        text += delim;
        text += "uniform sampler2D OverlayNormalSampler ";
        text += delim;
        text += "uniform half OverlayBumpiness ";
    }
    if (settings.texlayer_2.colorsampler.enabled)
    {
        text += delim;
        text += "uniform half OverlayStrength ";
    }
    if (settings.use_clut)
    {
        text += delim;
        text += "uniform sampler2D CLUTSampler ";
    }
    if (settings.glass_map)
    {
        text += delim;
        text += "uniform sampler2D BackBufferSampler ";
        text += delim;
        text += "uniform half GlassStrength ";
        text += delim;
        text += "uniform half Curvature ";
    }
    if (settings.parallax)
    {
        text += delim;
        text += "uniform half ParallaxBiasX ";
        text += delim;
        text += "uniform half ParallaxBiasY ";
    }
    if (settings.texlayer_1_distort)
    {
        text += delim;
        text += "uniform half DistortionStrength ";
    }
    if (settings.fog)
    {
        text += delim;
        text += "uniform half3 FogColor ";
    }
    if (settings.receive_shadows)
    {
        text += delim;
        text += "uniform sampler2D ShadowSampler ";
    }
    // SHADER INPUTS
    if (settings.use_vertex_colors)
    {
        text += delim;
        text += "in half4 vColor : COLOR0";
    }

    size_t num_texcoords = 0;
    for (size_t i = 0; i < settings.uv_slots.size(); ++i)
    {
        auto& slot = settings.uv_slots[i];
        if (slot.enabled)
        {
            text += delim;
            text += std::format("in half{0} vTexcoord{1} : TEXCOORD{2}", slot.vec_size, i, num_texcoords);
            ++num_texcoords;
        }
    }
    if (settings.use_normals)
    {
        text += delim;
        text += std::format("in half{0} vNormal : TEXCOORD{1}", settings.fragment_normal_vec_size, num_texcoords);
        ++num_texcoords;
    }
    if (settings.use_tangents)
    {
        text += delim;
        text += std::format("in half3 vTangent : TEXCOORD{0}", num_texcoords);
        ++num_texcoords;
        text += delim;
        text += std::format("in half3 vBinormal : TEXCOORD{0}", num_texcoords);
        ++num_texcoords;
    }
    if (settings.view_matrix)
    {
        text += delim;
        text += std::format("in half3 vView : TEXCOORD{0}", num_texcoords);
        ++num_texcoords;
    }
    if (settings.requires_screenpos)
    {
        text += delim;
        text += std::format("in half4 vScreenPos : TEXCOORD{0}", num_texcoords);
        ++num_texcoords;
    }
    if (settings.receive_shadows)
    {
        text += delim;
        text += std::format("in half4 vShadowMapCoord : TEXCOORD{0}", num_texcoords);
        ++num_texcoords;
    }
    if (settings.fog and not settings.texlayer_1.colorsampler.enabled)
    {
        text += delim;
        text += std::format("in half vFogFactor : TEXCOORD{0}", num_texcoords);
    }

    // SHADER OUTPUT
    text += delim;
    text += "out half4 oColor : COLOR";
    text += " ) {\n";

    return text;
}

std::string initSpecular(const FactorySettings& settings)
{
    std::string text;
    if (settings.use_specular)
        text += "\thalf4 specContrib = half4(0,0,0,0);\n";
    return text;
}

std::string initNormal(const FactorySettings& settings)
{
    std::string text;
    if (settings.use_normals)
        text += "\thalf3 Nn = vNormal.xyz;\n";
    return text;
}

std::string initParallax(const FactorySettings& settings)
{
    std::string text;
    if (settings.parallax)
    {
        text += "\thalf3 vv = mul(vView, half3x3( vTangent.x, vBinormal.x, Nn.x, vTangent.y, vBinormal.y, Nn.y, vTangent.z, vBinormal.z, Nn.z) );\n";

        std::string sampler;
        std::string tex_coord;
        std::string channel;
        switch (settings.parallax_heightmap)
        {
        case MapType::DIFFUSE:
            sampler = "ColorSampler";
            if (settings.texlayer_1.colorsampler.combined_channels)
                tex_coord = std::format("vTexcoord{0}.xy", settings.texlayer_1.colorsampler.uv_slot);
            else
                tex_coord = std::format("vTexcoord{0}.xy", "0");
            break;
        case MapType::NORMAL:
            sampler = "NormalSampler";
            if (settings.texlayer_1.normalsampler.combined_channels)
                tex_coord = std::format("vTexcoord{0}.xy", settings.texlayer_1.normalsampler.uv_slot);
            else
                tex_coord = std::format("vTexcoord{0}.xy", "0");
            break;
        default:
            throw std::exception("Bad Sampler selection in parallax generation.");
        }

        switch (settings.parallax_heightmap_channel)
        {
        case RGBAChannel::R:
            channel = "r";
            break;
        case RGBAChannel::G:
            channel = "g";
            break;
        case RGBAChannel::B:
            channel = "b";
            break;
        case RGBAChannel::A:
            channel = "a";
            break;
        default:
            throw std::exception("Bad channel selection in parallax generation.");
        }

        text += std::format("\thalf2 parallax = ( tex2D({0},{1}).{2} * ParallaxBiasX + ParallaxBiasY) * vv.xy;\n", sampler, tex_coord, channel);
    }
    return text;
}

std::string generateTextureSample(const std::string& sampler_name, const std::string& channels, const std::string& output_name, const std::string& space, const std::string& coordinate)
{
    return std::format("\t{4}{3} = tex2D({0},{1}{2}){3};\n", sampler_name, coordinate, space, channels, output_name);
}

std::string generateTexcoordTransform(const std::string& coordinate_name, bool use_distortion, const std::string& distortion_variable, bool use_parallax, const std::string& parallax_variable)
{
    std::string text;
    text += coordinate_name;
    if (use_parallax)
        text += " + " + parallax_variable;
    if (use_distortion)
        text += " +" + distortion_variable;

    return text;
}

std::string sampleTexture(const Sampler& sampler, const std::string& sampler_name, const std::string& output_name, bool use_space, bool use_distortion, const std::string& distortion_variable, bool use_parallax, const std::string& parallax_variable)
{
    std::string text;
    if (sampler.enabled)
    {
        std::string space;
        if (use_space)
            space = " ";
        else
            space = "";

        text += "\thalf4 " + output_name + ";\n";
        if (sampler.combined_channels)
        {
            text += generateTextureSample(sampler_name, "", output_name, space, generateTexcoordTransform(std::format("vTexcoord{0}.xy", sampler.uv_slot), use_distortion, distortion_variable, use_parallax, parallax_variable));
        }
        else
        {
            text += generateTextureSample(sampler_name, ".rgb", output_name, space, generateTexcoordTransform(std::format("vTexcoord{0}.xy", sampler.uv_slot), use_distortion, distortion_variable, use_parallax, parallax_variable));
            text += generateTextureSample(sampler_name, ".a", output_name, space, generateTexcoordTransform(std::format("vTexcoord{0}.xy", "0"), use_distortion, distortion_variable, use_parallax, parallax_variable));
        }
    }
    return text;
}

std::string generateDistortion(const FactorySettings& settings)
{
    if (settings.texlayer_1_distort)
        return "\thalf3 distortion = ( (normalRGBA.rgb) - half3(0.5,0.5,0.5) ) * DistortionStrength;\n";
    else
        return "";
}

std::string initOverlayStrength(const FactorySettings& settings)
{
    std::string text;
    bool use_overlay = settings.texlayer_2.colorsampler.enabled;
    if (use_overlay)
    {
        text += "\thalf overlayStlength = OverlayStrength;\n";
        text += std::format("\toverlayStlength *= {0};\n", parseMap(settings.layer_2_diffuse_map));
        if (settings.layer_2_vertex_alpha)
            text += "\toverlayStlength *= vColor.a;\n";
    }

    if (settings.use_tangents)
    {
        if (use_overlay)
            text += "\thalf3 bump = (normalRGBA.rgb - half3(0.5,0.5,0.5))*((1.0-overlayStlength)*Bumpiness);\n";
        else
            text += "\thalf3 bump = (normalRGBA.rgb - half3(0.5,0.5,0.5))*Bumpiness;\n";
        text += "\tNn = Nn + bump.x*vTangent + bump.y*vBinormal;\n";
    }
    if (settings.texlayer_2.normalsampler.enabled)
    {
        if (use_overlay)
            text += "\thalf3 olbump = (overlayNormalRGBA.rgb - half3(0.5,0.5,0.5))*(OverlayBumpiness*overlayStlength);\n";
        else
            text += "\thalf3 olbump = (overlayNormalRGBA.rgb - half3(0.5,0.5,0.5))*OverlayBumpiness;\n";

        text += "\tNn = Nn + olbump.x*vTangent + olbump.y*vBinormal;\n";
    }
    return text;

}

std::string normalizeNormals(const FactorySettings& settings)
{
    if (settings.use_normals)
        return "\tNn = normalize(Nn);\n";
    else
        return "";
}

std::string calculateDiffuseContribution(const FactorySettings& settings)
{
    std::string text;
    if (settings.texlayer_1.colorsampler.enabled)
        text += "\thalf4 diffuseColor = colorRGBA;\n";
    else
        text += "\thalf4 diffuseColor = DiffuseColor;\n";

    if (settings.layer_1_transparency_map != MapType::NONE)
    {
        std::string transparency_src;
        switch (settings.layer_1_transparency_map)
        {
        case (MapType::DIFFUSE):
            transparency_src = "colorRGBA";
            break;
        case (MapType::NORMAL):
            transparency_src = "normalRGBA";
            break;
        case (MapType::LIGHT):
            transparency_src = "lightRGBA";
            break;
        default:
            throw std::exception("Bad maptype in transparency map.");
        }

        std::string transparency_channel;
        switch (settings.layer_1_transparency_map_channel)
        {
        case (RGBAChannel::R):
            transparency_channel = "r";
            break;
        case (RGBAChannel::G):
            transparency_channel = "g";
            break;
        case (RGBAChannel::B):
            transparency_channel = "b";
            break;
        case (RGBAChannel::A):
            transparency_channel = "a";
            break;
        default:
            throw std::exception("Bad channel in transparency map.");
        }
        text += std::format("\tdiffuseColor.a = {0}.{1};\n", transparency_src, transparency_channel);
    }

    if (settings.texlayer_2.colorsampler.enabled)
    {
        text += "\tdiffuseColor.rgb = diffuseColor.rgb * (1.0-overlayStlength) + overlayColorRGBA.rgb * overlayStlength;\n";
    }


    if (settings.layer_1_vertex_rgb)
    {
        if (settings.layer_1_vertex_alpha)
            text += "\tdiffuseColor *= vColor;\n";
        else
            text += "\tdiffuseColor.rgb *= vColor.rgb;\n";
    }

    if (settings.texlayer_1.colorsampler.enabled)
    {
        if (settings.diffuse_map.enabled && settings.diffuse_map.type != MapType::NONE)
        {
            std::string map_src = parseMap(settings.diffuse_map);
            text += std::format("\tdiffuseColor *= (half4(1.0,1.0,1.0,1.0)+(DiffuseColor-half4(1.0,1.0,1.0,1.0))*{0});\n", map_src);
        }
        else
        {
            text += "\tdiffuseColor *= DiffuseColor;\n";
        }
    }

    if (settings.use_lightpower)
        text += "\tdiffuseColor.rgb *= lerp(half3(1.0,1.0,1.0),lightRGBA.rgb * LightMapPower,LightMapStrength);\n";

    return text;
}

std::string calculateSpecularContribution(const FactorySettings& settings)
{
    std::string text;
    if (settings.specular_input)
    {
        if (settings.specular_map.enabled)
            text += std::format("\thalf spStr = {0};\n", parseMap(settings.specular_map));
        else
            text += std::format("\thalf spStr = {0};\n", "SpecularStrength");

        // Something similar for layer 2, just with spStr = spStr(1.0-overlayStlength) + <map channel> * overlayStlength;"
    }

    // Lights
    if (settings.dir_light_1)
    {
        // Setup
        text += "\thalf3 diffTmp;\n";
        text += "\thalf3 Ln;\n";
        text += "\thalf3 Lc;\n";
        text += "\thalf l;\n";
        text += "\thalf ldn;\n";

        if (settings.specular_input)
        {
            text += "\thalf3 Hn;\n";
            if (settings.use_clut)
            {
                text += "\thalf2 clut;\n";
                text += "\thalf4 lightPow;\n";
            }
            else
            {
                text += "\thalf d;\n";
                text += "\thalf pw;\n";
                text += "\thalf specPow;\n";
            }
            text += "\thalf3 specTmp;\n";
        }
        else
        {
            if (settings.use_clut)
                text += "\thalf2 clut;\n";
        }

        if (settings.use_velvet)
        {
            text += "\thalf subLamb;\n";
            text += "\thalf3 subContrib;\n";
            if (settings.specular_input)
            {
                text += "\thalf vdn;\n";
                text += "\thalf3 vecColor;\n";
            }
        }

        // Light 1
        text += "\tLn = DirLamp01Dir;\n";
        text += "\tLc = DirLamp01Color.rgb;\n";
        text += "\tldn = dot(Ln,Nn);\n";

        if (!settings.use_clut)
            text += "\tldn = max(0.0,ldn);\n";

        if (settings.specular_input)
        {
            text += "\tHn = normalize(vView + Ln);\n";
            if (settings.use_clut)
            {
                text += "\tclut = half2(ldn*0.495+0.5, dot(Hn,Nn)*0.98+0.01);\n";
                text += "\tlightPow = tex2D(CLUTSampler, clut);\n";
                text += "\tdiffTmp = lightPow.rgb * Lc;\n";
                text += "\tspecTmp = lightPow.a * spStr * diffTmp;\n";
            }
            else
            {
                text += "\td = dot(Hn,Nn);\n";
                text += "\tpw = max( d/(SpecularPower *(1.0-d)+d),0.0);\n";
                text += "\tspecPow = step(0.0, ldn) * pw;\n";
                text += "\tdiffTmp = ldn * Lc;\n";
                text += "\tspecTmp = specPow * spStr * diffTmp;\n";
            }
        }
        else
        {
            if (settings.use_clut)
            {
                text += "\tclut = half2(ldn*0.495+0.5, 0.5);\n";
                text += "\tdiffTmp = tex2D(CLUTSampler, clut).xyz * Lc;\n";
            }
            else
            {
                text += "\tdiffTmp = ldn * Lc;\n";
            }
        }

        if (settings.use_velvet)
        {
            text += "\tdiffTmp *= SurfaceColor.rgb;\n";
            text += "\tsubLamb = smoothstep(-RollOff,1.0,ldn) - smoothstep(0.0,1.0,ldn);\n";
            text += "\tsubLamb = max(0.0,subLamb);\n";
            text += "\tsubContrib = subLamb * SubColor.rgb;\n";
            text += "\tdiffTmp += subContrib*VelvetStrength;\n";

            if (settings.specular_input)
            {
                text += "\tvdn = 1.0-dot(vView,Nn);\n";
                text += "\tvecColor = half3(vdn,vdn,vdn);\n";
                text += "\tspecTmp += (vecColor*FuzzySpecColor.rgb).xyz*VelvetStrength;\n";
            }
        }

        text += "\thalf3 diffContrib=diffTmp;\n";

        if (settings.specular_input)
            text += "\tspecContrib.rgb+=specTmp;\n";
    }

    if (settings.use_reflection)
    {
        if (settings.specular_map.enabled)
        {
            if (settings.specular_map.type == MapType::NORMAL)
                text += std::format("\thalf rfStr = normalRGBA.w;\n");
            else
                text += std::format("\thalf rfStr = diffuseColor.a;\n");
            text += "\trfStr *= ReflectionStrength;\n";
        }
        else
        {
            text += "\thalf rfStr = ReflectionStrength;\n";
        }

        if (settings.use_fresnel)
        {
            text += "\thalf KrMin = (rfStr * FresnelMin);\n";
            text += "\thalf InvFrExp = (1.0/FresnelExp);\n";
            text += "\thalf ft = pow(abs(dot(Nn,vView)),InvFrExp);\n";
            text += "\trfStr = lerp(rfStr,KrMin,ft);\n";
        }

        text += "\thalf3 reflVect = reflect(-vView,-Nn);\n";

        if (settings.texlayer_1_distort)
        {
            text += "\treflVect += distortion;\n";
            text += "\treflVect = normalize(reflVect);\n";
        }

        if (settings.refl_sphere_map)
        {
            text += "\thalf m = 2.0 * sqrt( reflVect.x*reflVect.x + reflVect.y*reflVect.y + (reflVect.z+1.0)*(reflVect.z+1.0) );\n";
            text += "\tfloat2 envuv = reflVect.xy/m + half2(0.5,0.5);\n";
            text += "\tspecContrib += tex2D(EnvsSampler,envuv ) * rfStr;\n";
        }
        else
        {
            text += "\tspecContrib += texCUBE(EnvSampler,reflVect) *rfStr;\n";
        }
    }

    return text;
}

std::string generateGlassmap(const FactorySettings& settings)
{
    std::string text;
    if (settings.glass_map)
    {
        text += "\thalf4 glassPos = vScreenPos;\n";
        text += "\tglassPos.xy += (vView.xy-Nn.xy)*(Curvature*vScreenPos.w);\n";
        text += "\tdiffuseColor.rgb = lerp(diffuseColor.rgb, tex2Dproj(BackBufferSampler, glassPos.xyw).rgb , GlassStrength);\n";
    }
    return text;
}

std::string initObscure(const FactorySettings& settings)
{
    if (settings.use_obscure)
        return "\thalf obscure = 1.0;\n";
    else
        return "";
}

std::string createShadows(const FactorySettings& settings)
{
    if (settings.receive_shadows)
        return "\tobscure = tex2Dproj(ShadowSampler, vShadowMapCoord.xyw).x;\n";
    else
        return "";
}

std::string createAmbientOcclusion(const FactorySettings& settings)
{
    if (settings.ambient_occlusion.enabled)
        return std::format("\tobscure *= {0};\n", parseMapWXYZ(settings.ambient_occlusion));
    else
        return "";
}

std::string createAmbientLight(const FactorySettings& settings)
{
    std::string text;
    bool ambient_contrib = settings.use_ambient;
    bool obscure_contrib = settings.use_obscure;
    if (settings.use_ambient)
    {
        if (settings.use_ambient_gndsky)
        {
            text += "\thalf hst = (dot(Nn,SkyDir.rgb) + 1.0)*0.5;\n";
            text += "\thalf3 ambient = lerp(GroundColor,AmbientColor,hst);\n";
        }
        else
        {
            text += "\thalf3 ambient = AmbientColor;\n";
        }
    }

    if (settings.dir_light_1)
    {
        std::string opstr = parseOp(settings.light_op);
        std::string contrib = "diffContrib";
        if (obscure_contrib)
            contrib += "*obscure";
        if (ambient_contrib)
            contrib += " + ambient";
        if (obscure_contrib || ambient_contrib)
            contrib = "(" + contrib + ")";

        text += std::format("\tdiffuseColor.rgb {0}= {1};\n", opstr, contrib);
    }
    else
    {
        if (ambient_contrib)
            text += "\tdiffuseColor.rgb *= (half3(1.0,1.0,1.0) + ambient);\n";
        else if (obscure_contrib)
            text += "\tdiffuseColor.rgb *= obscure;\n";
    }
    return text;
}

std::string applySpecular(const FactorySettings& settings)
{
    std::string text;
    if (settings.use_specular)
    {
        if (settings.use_obscure)
            text += "\tdiffuseColor.rgb += specContrib.rgb*obscure;\n";
        else
            text += "\tdiffuseColor.rgb += specContrib.rgb;\n";

        if (settings.use_specular_alpha)
            text += "\tdiffuseColor.a *= specContrib.a;";
    }
    return text;
}

std::string applyFog(const FactorySettings& settings)
{
    std::string text;
    if (settings.fog)
    {
        if (settings.use_normals)
            text += "\tfloat ff = vNormal.w;\n";
        else if (settings.texlayer_1.colorsampler.enabled)
            text += std::format("\tfloat ff = vTexcoord{0}.z;\n", settings.texlayer_1.colorsampler.uv_slot);
        else
            text += "\tfloat ff = vFogFactor;\n";

        text += "\tdiffuseColor.rgb = FogColor * ff + diffuseColor.rgb * (1.0-ff);\n";
    }
    return text;
}

std::string generateFragmentShader(const FactorySettings& settings)
{
    std::string text;
    text += createHeader(settings);
    text += initSpecular(settings);
    text += initNormal(settings);
    text += initParallax(settings);
    text += sampleTexture(settings.texlayer_2.normalsampler, "OverlayNormalSampler", "overlayNormalRGBA", true, false, "", false, "");
    text += sampleTexture(settings.texlayer_1.normalsampler, "NormalSampler", "normalRGBA", true, false, "", settings.parallax, "parallax");
    text += generateDistortion(settings);
    text += sampleTexture(settings.texlayer_1.colorsampler, "ColorSampler", "colorRGBA", true, settings.texlayer_1_distort, "distortion.xy", settings.parallax, "parallax");
    text += sampleTexture(settings.lightsampler, "LightSampler", "lightRGBA", false, false, "", false, "");
    text += sampleTexture(settings.texlayer_2.colorsampler, "OverlayColorSampler", "overlayColorRGBA", true, false, "", false, "");
    text += initOverlayStrength(settings);
    text += normalizeNormals(settings);
    text += calculateDiffuseContribution(settings);
    text += calculateSpecularContribution(settings);
    text += generateGlassmap(settings);
    // Set up obscure parameter
    text += initObscure(settings);
    text += createShadows(settings);
    text += createAmbientOcclusion(settings);
    // Use obscure parameter w/ lighting
    text += createAmbientLight(settings);
    text += applySpecular(settings);
    text += applyFog(settings);
    text += "\toColor = diffuseColor;\n";
    text += "}\n";
    return text;
}