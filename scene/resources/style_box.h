/**************************************************************************/
/*  style_box.h                                                           */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include "core/io/resource.h"
#include "core/object/gdvirtual.gen.inc"
#include "scene/animation/tween.h"

class CanvasItem;

class StyleBoxDrawModifier : public Resource {
	GDCLASS(StyleBoxDrawModifier, Resource);

protected:
	static void _bind_methods();

public:
	struct Data {
		Point2 pivot = Point2(.5, .5);
		Point2 offset = Point2();
		Size2 scale = Size2(1, 1);
		real_t rotation = 0.0;
		Color modulate = Color(1, 1, 1, 1);
	};
	Data data;

	void set_pivot(Point2 p_pivot);
	Point2 get_pivot() const;

	void set_offset(Point2 p_offset);
	Point2 get_offset() const;

	void set_scale(Size2 p_scale);
	Size2 get_scale() const;

	void set_rotation(real_t p_rotation);
	real_t get_rotation() const;

	void set_modulate(Color p_color);
	Color get_modulate() const;

	StyleBoxDrawModifier();
};

class StyleBoxAnimationInfo : public Resource {
	GDCLASS(StyleBoxAnimationInfo, Resource);

protected:
	static void _bind_methods();

public:
	struct Data {
		double duration = -1.0;
		Tween::EaseType ease = Tween::EASE_MAX;
		Tween::TransitionType transition = Tween::TRANS_MAX;
	};
	Data data;

	void set_duration(double p_duration);
	double get_duration() const;

	void set_ease(Tween::EaseType p_ease);
	Tween::EaseType get_ease() const;

	void set_transition(Tween::TransitionType p_transition);
	Tween::TransitionType get_transition() const;

	StyleBoxAnimationInfo();
};

class StyleBox : public Resource {
	GDCLASS(StyleBox, Resource);
	RES_BASE_EXTENSION("stylebox");
	OBJ_SAVE_TYPE(StyleBox);

	float content_margin[4];

protected:
	Ref<StyleBoxDrawModifier> normal_mod_ref;
	Ref<StyleBoxDrawModifier> entering_mod_ref;
	Ref<StyleBoxDrawModifier> exiting_mod_ref;

	StyleBoxDrawModifier::Data normal_modifier;
	StyleBoxDrawModifier::Data entering_modifier;
	StyleBoxDrawModifier::Data exiting_modifier;

	static void _bind_methods();
	virtual float get_style_margin(Side p_side) const { return 0; }

	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

	GDVIRTUAL2C_REQUIRED(_draw, RID, Rect2)
	GDVIRTUAL1RC(Rect2, _get_draw_rect, Rect2)
	GDVIRTUAL0RC(Size2, _get_minimum_size)
	GDVIRTUAL2RC(bool, _test_mask, Point2, Rect2)

public:
	struct AnimKey {
		ObjectID node_id;
		StringName group;

		AnimKey(ObjectID p_id, StringName p_group) {
			node_id = p_id;
			group = p_group;
		}

		// Needed for HashMap equality
		bool operator==(const AnimKey &other) const {
			return node_id == other.node_id && group == other.group;
		}
	};

	struct AnimKeyHasher {
		static _FORCE_INLINE_ uint32_t hash(const AnimKey &key) {
			uint32_t h = hash_djb2_one_64((uint64_t)key.node_id);
			h = hash_djb2_one_32(h, key.group.hash());
			return h;
		}
	};

	enum AnimationPhase {
		ENTERING,
		NORMAL,
		EXITING,
		KIND_MAX
	};

	struct AnimationValues {
		Variant from;
		Variant to;
		Variant current;
		real_t start_time = 0;
	};

	struct AnimationState {
		Ref<StyleBox> box = nullptr;
		Rect2 rect;
		Transform2D transform = Transform2D();
		Color modulate = Color(1, 1, 1, 1);
		AnimationPhase phase = NORMAL;
		bool has_drawn_box = false;

		uint64_t end_time = 0;
		uint64_t start_time = 0;
		uint64_t current_time = 0;

		HashMap<StringName, AnimationValues> values = {};
	};

	inline static HashMap<AnimKey, AnimationState, AnimKeyHasher> transition_groups;
	inline static StringName animation_id = "";
	inline static Rect2 current_draw_rect;

	inline static LocalVector<Transform2D> transform_stack;
	inline static LocalVector<Color> modulate_stack;
	inline static Transform2D total_transform = Transform2D();
	inline static Color total_modulate = Color(1, 1, 1, 1);

	inline static bool redrawer_connected = false;
	inline static bool is_drawing_exit = false;
	inline static bool is_drawing_enter = false;
	inline static AnimationState *cached_state = nullptr;

	HashMap<StringName, Ref<StyleBoxAnimationInfo>> animation_info_refs;
	HashMap<StringName, StyleBoxAnimationInfo::Data> animation_infos;
	StyleBoxAnimationInfo::Data animation_default = StyleBoxAnimationInfo::Data{ 0.0, Tween::EASE_OUT, Tween::TRANS_QUAD };
	real_t max_duration = 0.0;

	static void handle_node_redraws();

	static void enter_animation_group(StringName p_id);
	static void exit_animation_group(StringName p_id = "");

	static void apply_group_modifiers(StringName p_id);
	static void reset_modifiers();

	void begin_draw(RID p_canvas_item, const Rect2 &p_rect) const;
	void end_draw(RID p_canvas_item, const Rect2 &p_rect) const;

	void setup_animation() const;
	static Variant get_animated_value(StringName p_name, Variant p_default, StringName p_group = "", StringName p_info_prop = "");

	virtual LocalVector<StringName> get_animatable_props() const { return {}; }
	virtual Size2 get_minimum_size() const;

	void set_content_margin(Side p_side, float p_value);
	void set_content_margin_all(float p_value);
	void set_content_margin_individual(float p_left, float p_top, float p_right, float p_bottom);
	float get_content_margin(Side p_side) const;

	float get_margin(Side p_side) const;
	Point2 get_offset() const;

	void update_cached_modifiers();
	void set_draw_modifier(AnimationPhase p_phase, Ref<StyleBoxDrawModifier> p_modifier);
	Ref<StyleBoxDrawModifier> get_draw_modifier(AnimationPhase p_phase);

	virtual void draw(RID p_canvas_item, const Rect2 &p_rect) const;
	virtual Rect2 get_draw_rect(const Rect2 &p_rect) const;

	CanvasItem *get_current_item_drawn() const;

	virtual bool test_mask(const Point2 &p_point, const Rect2 &p_rect) const;

	StyleBox();
};

class StyleBoxEmpty : public StyleBox {
	GDCLASS(StyleBoxEmpty, StyleBox);
	virtual float get_style_margin(Side p_side) const override { return 0; }

public:
	virtual void draw(RID p_canvas_item, const Rect2 &p_rect) const override {}
};

VARIANT_ENUM_CAST(StyleBox::AnimationPhase);
