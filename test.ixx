module; 

export module test; 

import std.core; 

import vec; 
import material; 
import saveppm; 

struct Light 
{ 
    float intensity{}; 
    Vec<float> position{}; 

    Light(const Vec<float>& p, float i) : 
        position{p}, 
        intensity{i} {} 
};

struct Sphere 
{ 
    float radius{}; 
    Vec<float> center{}; 
    OpticalSurface<float> surf{}; 
    OpticalBulk<float> bulk{}; 

    Sphere(const Vec<float>& c, 
           float r, 
           const OpticalSurface<float>& surf_, 
           const OpticalBulk<float>& bulk_) : 
        center{c}, 
        radius{r}, 
        surf{surf_}, 
        bulk{bulk_} {} 

    bool ray_intersect(const Vec<float>& orig, 
                       const Vec<float>& dir, 
                       float& t0) const 
    {
        Vec<float> L = center - orig;
        float tca = L*dir;
        float d2 = L*L - tca*tca;
        if (d2 > radius*radius) 
            return false;
        float thc = std::sqrt(radius*radius - d2); 
        t0 = tca - thc;
        float t1 = tca + thc;
        if (t0 < 0) 
            t0 = t1;
        if (t0 < 0) 
            return false;
        return true;
    }
};

Vec<float> reflect(const Vec<float>& I, const Vec<float>& N) 
{
    return I - N*2.0f*(I*N);
}

Vec<float> refract( // Snell's law 
    const Vec<float>& I, 
    const Vec<float>& N, 
    float eta_t, 
    float eta_i = 1.0f 
) 
{ 
    float cosi = - std::max(-1.0f, std::min(1.0f, I*N));
    if (cosi < 0) 
        return refract(I, -N, eta_i, eta_t); // if the ray comes from the inside the object, swap the air and the media
    float eta = eta_i / eta_t;
    float k = 1 - eta*eta*(1 - cosi*cosi);
    return k < 0 ? Vec<float>{1.0f, 0.0f, 0.0f} : I*eta + N*(eta*cosi - std::sqrt(k)); // k < 0 = total reflection, no ray to refract. I refract it anyways, this has no physical meaning
}

bool scene_intersect( 
    const Vec<float>& orig, 
    const Vec<float>& dir, 
    const std::vector<Sphere>& spheres, 
    Vec<float>& hit, 
    Vec<float>& N, 
    OpticalSurface<float>& surf_, 
    OpticalBulk<float>& bulk_ 
) 
{
    float spheres_dist = std::numeric_limits<float>::max();
    for (size_t i = 0; i < spheres.size(); ++i) 
    {
        float dist_i;
        if (spheres[i].ray_intersect(orig, dir, dist_i) && dist_i < spheres_dist) 
        {
            spheres_dist = dist_i;
            hit = orig + dir*dist_i;
            N = (hit - spheres[i].center).normalize(); 
            surf_ = spheres[i].surf; 
            bulk_ = spheres[i].bulk; 
        }
    }

    float checkerboard_dist = std::numeric_limits<float>::max();
    if (std::fabs(dir.y) > 1e-3) 
    {
        float d = -(orig.y + 4)/dir.y; // the checkerboard plane has equation y = -4
        Vec<float> pt = orig + dir*d;
        if ((d > 0.0f) && (std::fabs(pt.x) < 10.0f) && (pt.z < -10.0f) && (pt.z > -30.0f) && (d < spheres_dist)) 
        {
            checkerboard_dist = d;
            hit = pt;
            N = Vec<float>{0.0f, 1.0f, 0.0f}; 
            surf_.color = (int(0.5f*hit.x + 1000) + int(0.5f*hit.z)) & 1 ? Vec<float>{0.3f, 0.3f, 0.3f} : Vec<float>{0.3f, 0.2f, 0.1f}; 
        }
    } 
    return std::min(spheres_dist, checkerboard_dist) < 1000.0f; 
}

Vec<float> cast_ray( 
    const Vec<float>& orig, 
    const Vec<float>& dir, 
    const std::vector<Sphere>& spheres, 
    const std::vector<Light>& lights, 
    size_t depth = 0) 
{
    Vec<float> point, N; 
    OpticalSurface<float> surf; 
    OpticalBulk<float> bulk; 

    if (depth > 4 || !scene_intersect(orig, dir, spheres, point, N, surf, bulk)) 
    {
        return Vec<float>{0.2f, 0.7f, 0.8f}; // background color
    }

    Vec<float> reflect_dir = reflect(dir, N).normalize();
    Vec<float> refract_dir = refract(dir, N, bulk.refraction).normalize();
    Vec<float> reflect_orig = reflect_dir*N < 0 ? point - N*1e-3 : point + N*1e-3; // offset the original point to avoid occlusion by the object itself
    Vec<float> refract_orig = refract_dir*N < 0 ? point - N*1e-3 : point + N*1e-3;
    Vec<float> reflect_color = cast_ray(reflect_orig, reflect_dir, spheres, lights, depth + 1);
    Vec<float> refract_color = cast_ray(refract_orig, refract_dir, spheres, lights, depth + 1);

    float diffuse_light_intensity = 0.0f, specular_light_intensity = 0.0f;
    for (size_t i = 0; i < lights.size(); ++i) 
    {
        Vec<float> light_dir = (lights[i].position - point).normalize();
        float light_distance = (lights[i].position - point).length(); 

        Vec<float> shadow_orig = light_dir*N < 0 ? point - N*1e-3 : point + N*1e-3; // checking if the point lies in the shadow of the lights[i]
        Vec<float> shadow_pt, shadow_N{}; 
        OpticalSurface<float> tmp_surf{}; 
        OpticalBulk<float> tmp_bulk{}; 
        if (scene_intersect(shadow_orig, light_dir, spheres, shadow_pt, shadow_N, tmp_surf, tmp_bulk) && (shadow_pt - shadow_orig).length() < light_distance)
            continue;

        diffuse_light_intensity  += lights[i].intensity * std::max(0.0f, light_dir*N);
        specular_light_intensity += std::pow(std::max(0.0f, -reflect(-light_dir, N)*dir), surf.specular)*lights[i].intensity;
    }
    return surf.color*diffuse_light_intensity*surf.reflection[0] + Vec<float>{1.0f, 1.0f, 1.0f}*specular_light_intensity*surf.reflection[1] + reflect_color*surf.reflection[2] + refract_color*surf.reflection[3];
}

void render(const std::vector<Sphere>& spheres, const std::vector<Light>& lights) 
{
    const int width = 1024;
    const int height = 768;
    const float fov = 3.14f/3.0f;
    std::vector< Vec<float> > framebuffer(width*height);

    for (size_t j = 0; j < height; ++j) // actual rendering loop 
    { 
        for (size_t i = 0; i < width; ++i) 
        {
            float dir_x =  (i + 0.5f) -  width/2.0f;
            float dir_y = -(j + 0.5f) + height/2.0f;    // this flips the image at the same time
            float dir_z = -height/(2.0f*std::tan(fov/2.0f));
            framebuffer[i + j*width] = cast_ray(Vec<float>{0.0f, 0.0f, 0.0f}, Vec<float>{dir_x, dir_y, dir_z}.normalize(), spheres, lights);
        }
    } 

    save_ppm(width, height, framebuffer); // save the framebuffer to file 
}

export void create_pic() 
{ 
    OpticalSurface<float> ivory_surf{50.0f, Vec<float>{0.6f, 0.3f, 0.1f, 0.0f}, Vec<float>{0.4f, 0.4f, 0.3f}}; 
    OpticalBulk<float> ivory_bulk{1.0f, 1.0f}; 
    OpticalSurface<float> glass_surf{125.0f, Vec<float>{0.0f, 0.5f, 0.1f, 0.8f}, Vec<float>{0.6f, 0.7f, 0.8f}}; 
    OpticalBulk<float> glass_bulk{1.5f, 1.0f}; 
    OpticalSurface<float> red_rubber_surf{10.0f, Vec<float>{0.9f, 0.1f, 0.0f, 0.0f}, Vec<float>{0.3f, 0.1f, 0.1f}}; 
    OpticalBulk<float> red_rubber_bulk{1.0f, 1.0f}; 
    OpticalSurface<float> mirror_surface{1425.0f, Vec<float>{0.0f, 10.0f, 0.8f, 0.0f}, Vec<float>{1.0f, 1.0f, 0.1f}}; 
    OpticalBulk<float> mirror_bulk{1.0f, 1.0f}; 

    std::vector<Sphere> spheres; 
    spheres.push_back(Sphere{Vec<float>{-3.0f, 0.0f, -16.0f}, 2.0f, ivory_surf, ivory_bulk}); 
    spheres.push_back(Sphere{Vec<float>{-1.0f, -1.5f, -12.0f}, 2.0f, glass_surf, glass_bulk}); 
    spheres.push_back(Sphere{Vec<float>{1.5f, -0.5f, -18.0f}, 3.0f, red_rubber_surf, red_rubber_bulk}); 
    spheres.push_back(Sphere{Vec<float>{7.0f, 5.0f, -18.0f}, 4.0f, mirror_surface, mirror_bulk}); 

    std::vector<Light> lights; 
    lights.push_back(Light{Vec<float>{-20.0f, 20.0f, 20.0f}, 1.5f}); 
    lights.push_back(Light{Vec<float>{30.0f, 50.0f, -25.0f}, 1.8f}); 
    lights.push_back(Light{Vec<float>{30.0f, 20.0f, 30.0f}, 1.7f}); 

    render(spheres, lights); 
} 
