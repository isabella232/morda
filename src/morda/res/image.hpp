/*
morda - GUI framework

Copyright (C) 2012-2021  Ivan Gagis <igagis@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* ================ LICENSE END ================ */

#pragma once

#include <array>

#include <r4/rectangle.hpp>

#include "texture.hpp"

#include "../resource_loader.hpp"

#include "../context.hpp"

namespace morda{ namespace res{

/**
 * @brief Base image resource class.
 * 
 * %resource description:
 * 
 * @param file - name of the file to read the image from, can be raster image or SVG.
 * 
 * Example:
 * @code
 * img_dropdown_arrow{
 *     file{dropdown_arrow.svg}
 * }
 * @endcode
 */
class image : public resource{
	friend class morda::resource_loader;
	
protected:
	image(std::shared_ptr<morda::context> c);
	
public:
	image(const image& orig) = delete;
	image& operator=(const image& orig) = delete;
	
	/**
	 * @brief Texture created from the image resource.
	 * The texture which was created from an image resource.
	 * This texture is to be used on a quad to render the image.
	 */
	class texture{
	protected:
		const std::shared_ptr<morda::renderer> renderer;

		texture(std::shared_ptr<morda::renderer> r, vector2 dims) :
				renderer(std::move(r)),
				dims(dims)
		{}
	public:
		const vector2 dims;
		
		virtual ~texture()noexcept{}

		void render(const matrix4& matrix)const{
			this->render(matrix, *this->renderer->pos_tex_quad_01_vao);
		}

		/**
		 * @brief Render a quad with this texture.
		 * @param matrix - transformation matrix to use for rendering.
		 * @param vao - vertex array to use for rendering.
		 */
		virtual void render(const matrix4& matrix, const vertex_array& vao)const = 0;
	};

	/**
	 * @brief Get dimensions of this image in pixels for given Dots Per Inch resolution.
	 * @param dpi - dots per inch.
	 * @return Dimensions of the image in pixels.
	 */
	virtual vector2 dims(real dpi)const noexcept = 0;
	
	vector2 dims()const noexcept{
		return this->dims(this->context->units.dots_per_inch);
	}

	/**
	 * @brief Get raster texture of given dimensions.
	 * @param forDims - dimensions request for raster texture.
	 *        If any of the dimensions is 0 then it will be adjusted to preserve aspect ratio.
	 *        If both dimensions are zero, then dimensions which are natural for the particular image will be used.
	 */
	virtual std::shared_ptr<const texture> get(vector2 forDims = 0)const = 0;
private:
	static std::shared_ptr<image> load(morda::context& ctx, const ::treeml::forest& desc, const papki::file& fi);
	
public:
	/**
	 * @brief Load image resource from image file.
	 * Files supported are PNG, JPG, SVG.
	 * @param ctx - context.
	 * @param fi - image file.
	 * @return Loaded resource.
	 */
	static std::shared_ptr<image> load(morda::context& ctx, const papki::file& fi);
};

/**
 * @brief Undocumented.
 */
class atlas_image : public image, public image::texture{
	friend class image;
	
	std::shared_ptr<res::texture> tex;
	
	std::shared_ptr<vertex_array> vao;
	
public:
	atlas_image(std::shared_ptr<morda::context> c, std::shared_ptr<res::texture> tex, const rectangle& rect);
	atlas_image(std::shared_ptr<morda::context> c, std::shared_ptr<res::texture> tex);
	
	atlas_image(const atlas_image& orig) = delete;
	atlas_image& operator=(const atlas_image& orig) = delete;
	
	vector2 dims(real dpi) const noexcept override{
		return this->image::texture::dims;
	}
	
	virtual std::shared_ptr<const image::texture> get(vector2 forDim)const override;
	
	void render(const matrix4& matrix, const vertex_array& vao) const override;
	
private:
	static std::shared_ptr<atlas_image> load(morda::context& ctx, const ::treeml::forest& desc, const papki::file& fi);
};

}}
