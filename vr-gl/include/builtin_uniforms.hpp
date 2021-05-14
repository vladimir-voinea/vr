#pragma once

namespace vr::gl
{
	constexpr auto builtin_vertex_position_attribute_name = "vr_vertex_position";
	constexpr auto builtin_vertex_normal_attribute_name = "vr_vertex_normal";
	constexpr auto builtin_vertex_color_attribute_name = "vr_vertex_color";
	constexpr auto builtin_vertex_uv_attribute_name = "vr_vertex_uv";

	constexpr auto builtin_view_position_uniform_name = "vr_view_position";
	constexpr auto builtin_mvp_uniform_name = "vr_mvp";
	constexpr auto builtin_projection_uniform_name = "vr_projection";
	constexpr auto builtin_view_uniform_name = "vr_view";
	constexpr auto builtin_model_uniform_name = "vr_model";
	constexpr auto builtin_modelview_uniform_name = "vr_modelview";
	constexpr auto builtin_normal_uniform_name = "vr_normal";

	constexpr auto builtin_have_shininess_uniform_name = "vr_material.have_shininess";
}