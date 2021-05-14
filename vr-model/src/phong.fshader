R"(
#version 300 es

struct vr_material_t
{
	highp bool have_ambient_color;
	highp vec3 ambient_color;
	highp bool have_ambient_texture;
	sampler2D ambient_texture;
				
	highp bool have_diffuse_color;
	highp vec3 diffuse_color;
	highp bool have_diffuse_texture;
	sampler2D diffuse_texture;

	highp bool have_specular_color;
	highp vec3 specular_color;
	highp bool have_specular_texture;
	sampler2D specular_texture;

	highp bool have_shininess;
	highp float shininess;
};

struct vr_light_components_t
{
	highp vec3 ambient;
	highp vec3 diffuse;
	highp vec3 specular;
};

struct vr_light_attenuation_t
{
	highp float constant;
	highp float linear;
	highp float quadratic;
};

struct vr_ambient_light_t
{
	highp vec3 color;
	highp float intensity;
};

struct vr_directional_light_t
{
	highp vec3 direction;
	vr_light_components_t components;
};

struct vr_point_light_t
{
	highp vec3 position;
	vr_light_attenuation_t attenuation;
	vr_light_components_t components;
};

struct vr_spot_light_t
{
	highp vec3 position;
	highp vec3 direction;
	highp float cutoff_cosine;
	highp float outer_cutoff_cosine;
	vr_light_attenuation_t attenuation;
	vr_light_components_t components;
};

in highp vec2 v_uv;
in highp vec3 v_normal;
in highp vec3 v_position;
out highp vec4 out_color4;

uniform highp vec3 vr_view_position;
uniform vr_material_t vr_material;

uniform bool vr_have_ambient_light;
uniform vr_ambient_light_t vr_ambient_light;

uniform bool vr_have_directional_light;
uniform vr_directional_light_t vr_directional_light;

uniform bool vr_have_point_light;
uniform vr_point_light_t vr_point_light;

uniform bool vr_have_spot_light;
uniform vr_spot_light_t vr_spot_light;

struct vr_color_components_t
{
	highp vec3 ambient;
	highp vec3 diffuse;
	highp vec3 specular;
};

#define DEFAULT_COLOR vec3(0.f, 0.f, 0.f)

highp float get_ambient_coefficient()
{
	highp float coefficient = 1.f;
	return coefficient;
}

highp vec3 get_ambient_color_contribution()
{
	return vr_material.have_ambient_color ? vr_material.ambient_color : DEFAULT_COLOR;
}

highp vec3 get_ambient_texture_contribution(highp vec2 uv)
{
	return texture(vr_material.ambient_texture, uv).rgb;
}

highp vec3 get_ambient_color(highp vec2 uv)
{
	return get_ambient_texture_contribution(uv) * get_ambient_color_contribution();
}

highp float get_diffuse_coefficient(highp vec3 normalized_normal, highp vec3 light_direction)
{
	highp float diff = max(dot(normalized_normal, light_direction), 0.f);
	return diff;
}

highp vec3 get_diffuse_color_contribution()
{
	return vr_material.have_diffuse_color ? vr_material.diffuse_color : DEFAULT_COLOR;
}

highp vec3 get_diffuse_texture_contribution(highp vec2 uv)
{
	return texture(vr_material.diffuse_texture, uv).rgb;
}

highp vec3 get_diffuse_color(highp vec2 uv)
{
	return get_diffuse_texture_contribution(uv) * get_diffuse_color_contribution();
}

highp float get_specular_coefficient(highp vec3 normalized_normal, highp vec3 light_direction, highp vec3 view_direction)
{
	highp vec3 reflect_direction = reflect(-light_direction, normalized_normal);
	highp float spec = pow(max(dot(view_direction, reflect_direction), 0.f), vr_material.shininess);
	return spec;
}

highp vec3 get_specular_color_contribution()
{
	return vr_material.have_specular_color ? vr_material.specular_color : DEFAULT_COLOR;
}

highp vec3 get_specular_texture_contribution(highp vec2 uv)
{
	return texture(vr_material.specular_texture, uv).rgb;
}

highp vec3 get_specular_color(highp vec2 uv)
{
	return get_specular_texture_contribution(uv) * get_specular_color_contribution();
}

vr_color_components_t get_color(highp vec2 uv)
{
	vr_color_components_t color;
	color.ambient = get_ambient_color(uv);
	color.diffuse = get_diffuse_color(uv);
	color.specular = get_specular_color(uv);

	return color;
}

highp vec3 add_ambient_light(vr_ambient_light_t light, vr_color_components_t color)
{
	highp vec3 ambient = light.color * light.intensity * color.diffuse;

	return ambient;
}

highp vec3 add_directional_light(vr_directional_light_t light, vr_color_components_t color, highp vec3 normal, highp vec3 view_direction)
{
	highp vec3 light_direction = normalize(-light.direction);

	highp vec3 ambient = light.components.ambient * get_ambient_coefficient() * color.ambient;
	highp vec3 diffuse = light.components.diffuse * get_diffuse_coefficient(normal, light_direction) * color.diffuse;
	highp vec3 specular = light.components.specular * get_specular_coefficient(normal, light_direction, view_direction) * color.specular;

	return (ambient + diffuse + specular);
}

highp float calculate_attenuation(vr_light_attenuation_t attenuation_data, highp float distance)
{
	highp float attenuation = 1.0 / (attenuation_data.constant + attenuation_data.linear * distance + attenuation_data.quadratic * (distance * distance));

	return attenuation;
}

highp vec3 add_point_light(vr_point_light_t light, vr_color_components_t color, highp vec3 normal, highp vec3 fragment_position, highp vec3 view_direction)
{
	highp vec3 unnormalized_light_direction = light.position - fragment_position;
	highp vec3 light_direction = normalize(unnormalized_light_direction);
	highp float distance = length(unnormalized_light_direction);
	highp float attenuation = calculate_attenuation(light.attenuation, distance);

	highp vec3 ambient = attenuation * light.components.ambient * get_ambient_coefficient() * color.ambient;
	highp vec3 diffuse = attenuation * light.components.diffuse * get_diffuse_coefficient(normal, light_direction) * color.diffuse;
	highp vec3 specular = attenuation * light.components.specular * get_specular_coefficient(normal, light_direction, view_direction) * color.specular;

	return (ambient + diffuse + specular);
}

highp vec3 add_spot_light(vr_spot_light_t light, vr_color_components_t color, highp vec3 normal, highp vec3 fragment_position, highp vec3 view_direction)
{
	highp vec3 unnormalized_light_direction = light.position - fragment_position;
	highp vec3 light_direction = normalize(unnormalized_light_direction);
	highp float distance = length(unnormalized_light_direction);
	highp float attenuation = calculate_attenuation(light.attenuation, distance);

	highp float theta = dot(light_direction, normalize(-light.direction));
	highp float epsilon = light.cutoff_cosine - light.outer_cutoff_cosine;
	highp float intensity = clamp((theta - light.outer_cutoff_cosine) / epsilon, 0.f, 1.f);

	highp vec3 ambient = intensity * attenuation * light.components.ambient * get_ambient_coefficient() * color.ambient;
	highp vec3 diffuse = intensity * attenuation * light.components.diffuse * get_diffuse_coefficient(normal, light_direction) * color.diffuse;
	highp vec3 specular = intensity * attenuation * light.components.specular * get_specular_coefficient(normal, light_direction, view_direction) * color.specular;

	return (ambient + diffuse + specular);
}

void main()
{
	highp vec3 normalized_normal = normalize(v_normal);
	highp vec3 view_direction = normalize(vr_view_position - v_position);

	highp vec3 accumulator = vec3(0.f, 0.f, 0.f);
	vr_color_components_t color = get_color(v_uv);
	
	if(vr_have_ambient_light)
	{
		highp vec3 with_ambient_light = add_ambient_light(vr_ambient_light, color);
		accumulator += with_ambient_light;
	}
	if(vr_have_directional_light)
	{
		highp vec3 with_directional_light = add_directional_light(vr_directional_light, color, normalized_normal, view_direction);
		accumulator += with_directional_light;
	}

	if(vr_have_point_light)
	{
		highp vec3 with_point_light = add_point_light(vr_point_light, color, normalized_normal, v_position, view_direction);
		accumulator += with_point_light;
	}
	
	if(vr_have_spot_light)
	{
		highp vec3 with_spot_light = add_spot_light(vr_spot_light, color, normalized_normal, v_position, view_direction);
		accumulator += with_spot_light;
	}

	highp vec3 min = vec3(0.f, 0.f, 0.f);
	highp vec3 max = vec3(1.f, 1.f, 1.f);

	out_color4 = vec4(clamp(accumulator, min, max), 1.f);
}
)"