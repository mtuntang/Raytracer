#ifndef MATERIAL_H
#define MATERIAL_H


class hit_record;

class material {
public:
	virtual ~material() = default;

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

/*
* For "matte" looking materials, think of stone pots or non-reflective surfaces
*/
class lambertian : public material {
public:
    lambertian(const color& a) : albedo(a) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
        auto scatter_direction = rec.normal + random_unit_vector();
        // Avoids scenario where random vec generated equals to -normal vec, hence scatter direction ends up being 0
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

class metal : public material {
public:
    metal(const color& albedo) : albedo(albedo) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};
#endif