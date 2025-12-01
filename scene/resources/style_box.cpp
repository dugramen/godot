/**************************************************************************/
/*  style_box.cpp                                                         */
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

#include "style_box.h"

#include "animation.h"
#include "scene/gui/control.h"
#include "scene/main/canvas_item.h"

void StyleBoxDrawModifier::set_pivot(Point2 p_pivot) {
	data.pivot = p_pivot;
	emit_changed();
}

Point2 StyleBoxDrawModifier::get_pivot() const {
	return data.pivot;
}

void StyleBoxDrawModifier::set_offset(Point2 p_offset) {
	data.offset = p_offset;
	emit_changed();
}

Point2 StyleBoxDrawModifier::get_offset() const {
	return data.offset;
}

void StyleBoxDrawModifier::set_scale(Size2 p_scale) {
	data.scale = p_scale;
	emit_changed();
}

Size2 StyleBoxDrawModifier::get_scale() const {
	return data.scale;
}

void StyleBoxDrawModifier::set_rotation(real_t p_rotation) {
	data.rotation = p_rotation;
	emit_changed();
}

real_t StyleBoxDrawModifier::get_rotation() const {
	return data.rotation;
}

void StyleBoxDrawModifier::set_modulate(Color p_modulate) {
	data.modulate = p_modulate;
	emit_changed();
}

Color StyleBoxDrawModifier::get_modulate() const {
	return data.modulate;
}

StyleBoxDrawModifier::StyleBoxDrawModifier() {}

void StyleBoxDrawModifier::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_pivot", "pivot"), &StyleBoxDrawModifier::set_pivot);
	ClassDB::bind_method(D_METHOD("get_pivot"), &StyleBoxDrawModifier::get_pivot);

	ClassDB::bind_method(D_METHOD("set_offset", "offset"), &StyleBoxDrawModifier::set_offset);
	ClassDB::bind_method(D_METHOD("get_offset"), &StyleBoxDrawModifier::get_offset);

	ClassDB::bind_method(D_METHOD("set_scale", "scale"), &StyleBoxDrawModifier::set_scale);
	ClassDB::bind_method(D_METHOD("get_scale"), &StyleBoxDrawModifier::get_scale);

	ClassDB::bind_method(D_METHOD("set_rotation", "rotation"), &StyleBoxDrawModifier::set_rotation);
	ClassDB::bind_method(D_METHOD("get_rotation"), &StyleBoxDrawModifier::get_rotation);

	ClassDB::bind_method(D_METHOD("set_modulate", "modulate"), &StyleBoxDrawModifier::set_modulate);
	ClassDB::bind_method(D_METHOD("get_modulate"), &StyleBoxDrawModifier::get_modulate);

	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "modulate"), "set_modulate", "get_modulate");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "pivot", PROPERTY_HINT_RANGE, "0,1,.1,or_less,or_greater"), "set_pivot", "get_pivot");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "offset"), "set_offset", "get_offset");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "scale", PROPERTY_HINT_RANGE, "0,2,.1,or_less,or_greater"), "set_scale", "get_scale");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "rotation", PROPERTY_HINT_RANGE, "-180,180,1,radians_as_degrees"), "set_rotation", "get_rotation");
}

void StyleBoxAnimationInfo::set_duration(double p_duration) {
	data.duration = p_duration;
	emit_changed();
}

double StyleBoxAnimationInfo::get_duration() const {
	return data.duration;
}

void StyleBoxAnimationInfo::set_ease(Tween::EaseType p_ease) {
	data.ease = p_ease;
	emit_changed();
}

Tween::EaseType StyleBoxAnimationInfo::get_ease() const {
	return data.ease;
}

void StyleBoxAnimationInfo::set_transition(Tween::TransitionType p_transition) {
	data.transition = p_transition;
	emit_changed();
}

Tween::TransitionType StyleBoxAnimationInfo::get_transition() const {
	return data.transition;
}

StyleBoxAnimationInfo::StyleBoxAnimationInfo() {}

void StyleBoxAnimationInfo::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_duration", "duration"), &StyleBoxAnimationInfo::set_duration);
	ClassDB::bind_method(D_METHOD("get_duration"), &StyleBoxAnimationInfo::get_duration);

	ClassDB::bind_method(D_METHOD("set_ease", "ease"), &StyleBoxAnimationInfo::set_ease);
	ClassDB::bind_method(D_METHOD("get_ease"), &StyleBoxAnimationInfo::get_ease);

	ClassDB::bind_method(D_METHOD("set_transition", "transition"), &StyleBoxAnimationInfo::set_transition);
	ClassDB::bind_method(D_METHOD("get_transition"), &StyleBoxAnimationInfo::get_transition);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "duration", PROPERTY_HINT_RANGE, "0,1,.05,or_greater,or_less"), "set_duration", "get_duration");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "ease", PROPERTY_HINT_ENUM, "EaseIn,EaseOut,EaseInOut,EaseOutIn,Unset"), "set_ease", "get_ease");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "transition", PROPERTY_HINT_ENUM, "Linear,Sine,Quint,Quart,Quad,Expo,Elastic,Cubic,Circ,Bounce,Back,Spring,Unset"), "set_transition", "get_transition");
}

Size2 StyleBox::get_minimum_size() const {
	Size2 min_size = Size2(get_margin(SIDE_LEFT) + get_margin(SIDE_RIGHT), get_margin(SIDE_TOP) + get_margin(SIDE_BOTTOM));
	Size2 custom_size;
	GDVIRTUAL_CALL(_get_minimum_size, custom_size);

	if (min_size.x < custom_size.x) {
		min_size.x = custom_size.x;
	}
	if (min_size.y < custom_size.y) {
		min_size.y = custom_size.y;
	}

	return min_size;
}

void StyleBox::set_content_margin(Side p_side, float p_value) {
	ERR_FAIL_INDEX((int)p_side, 4);

	content_margin[p_side] = p_value;
	emit_changed();
}

void StyleBox::set_content_margin_all(float p_value) {
	for (int i = 0; i < 4; i++) {
		content_margin[i] = p_value;
	}
	emit_changed();
}

void StyleBox::set_content_margin_individual(float p_left, float p_top, float p_right, float p_bottom) {
	content_margin[SIDE_LEFT] = p_left;
	content_margin[SIDE_TOP] = p_top;
	content_margin[SIDE_RIGHT] = p_right;
	content_margin[SIDE_BOTTOM] = p_bottom;
	emit_changed();
}

float StyleBox::get_content_margin(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0.0);

	return content_margin[p_side];
}

float StyleBox::get_margin(Side p_side) const {
	ERR_FAIL_INDEX_V((int)p_side, 4, 0.0);

	if (content_margin[p_side] < 0) {
		return get_style_margin(p_side);
	} else {
		return content_margin[p_side];
	}
}

Point2 StyleBox::get_offset() const {
	return Point2(get_margin(SIDE_LEFT), get_margin(SIDE_TOP));
}

void StyleBox::update_cached_modifiers() {
	if (entering_mod_ref.is_valid()) {
		entering_modifier.modulate = entering_mod_ref->get_modulate();
		entering_modifier.pivot = entering_mod_ref->get_pivot();
		entering_modifier.offset = entering_mod_ref->get_offset();
		entering_modifier.scale = entering_mod_ref->get_scale();
		entering_modifier.rotation = entering_mod_ref->get_rotation();
	}

	if (normal_mod_ref.is_valid()) {
		normal_modifier.modulate = normal_mod_ref->get_modulate();
		normal_modifier.pivot = normal_mod_ref->get_pivot();
		normal_modifier.offset = normal_mod_ref->get_offset();
		normal_modifier.scale = normal_mod_ref->get_scale();
		normal_modifier.rotation = normal_mod_ref->get_rotation();
	}

	if (exiting_mod_ref.is_valid()) {
		exiting_modifier.modulate = exiting_mod_ref->get_modulate();
		exiting_modifier.pivot = exiting_mod_ref->get_pivot();
		exiting_modifier.offset = exiting_mod_ref->get_offset();
		exiting_modifier.scale = exiting_mod_ref->get_scale();
		exiting_modifier.rotation = exiting_mod_ref->get_rotation();
	}

	emit_changed();
}

void StyleBox::set_draw_modifier(AnimationPhase p_phase, Ref<StyleBoxDrawModifier> p_modifier) {
	if (p_phase == ENTERING) {
		entering_mod_ref = p_modifier;
	} else if (p_phase == EXITING) {
		exiting_mod_ref = p_modifier;
	} else {
		normal_mod_ref = p_modifier;
	}
	update_cached_modifiers();
	if (p_modifier.is_valid() && !p_modifier->is_connected(SNAME("changed"), callable_mp(this, &StyleBox::update_cached_modifiers))) {
		p_modifier->connect(SNAME("changed"), callable_mp(this, &StyleBox::update_cached_modifiers));
	}
}

Ref<StyleBoxDrawModifier> StyleBox::get_draw_modifier(AnimationPhase p_phase) {
	return p_phase == ENTERING
			? entering_mod_ref
			: p_phase == EXITING
			? exiting_mod_ref
			: normal_mod_ref;
}

void StyleBox::enter_animation_group(StringName p_id) {
	animation_id = p_id;
	cached_state = nullptr;
}

void StyleBox::exit_animation_group(StringName p_id) {
	Control *node = cast_to<Control>(CanvasItem::get_current_item_drawn());
	if (!node) {
		return;
	}

	if (!p_id.is_empty()) {
		animation_id = p_id;
	}
	ObjectID nid = node->get_instance_id();
	AnimKey key = AnimKey(nid, animation_id);

	if (transition_groups.has(key)) {
		AnimationState &current_state = transition_groups[key];
		uint64_t current_time = OS::get_singleton()->get_ticks_usec();
		current_state.current_time = current_time;

		// Initialize start of exit animation
		if (current_state.box.is_valid() && !current_state.has_drawn_box && current_state.phase != EXITING) {
			current_state.phase = EXITING;
			if (current_state.box->exiting_mod_ref.is_valid()) {
				current_state.end_time = current_time + 1;
			}
		}
		current_state.has_drawn_box = false;

		// Draw exit stylebox every frame of duration
		if (current_state.phase == EXITING) {
			if (current_time < current_state.end_time && current_state.box.is_valid()) {
				bool cached_exiting = is_drawing_exit;
				is_drawing_exit = true;
				current_state.box->draw(node->get_canvas_item(), current_state.rect);
				is_drawing_exit = cached_exiting;
			}
		}
	}

	animation_id = "";
	cached_state = nullptr;
}

void StyleBox::handle_node_redraws() {
	for (HashMap<AnimKey, AnimationState, AnimKeyHasher>::Iterator it = transition_groups.begin(); it;) {
		Control *node = cast_to<Control>(ObjectDB::get_instance(it->key.node_id));
		if (node) {
			if (it->value.current_time < it->value.end_time) {
				node->queue_redraw();
			} else if (it->value.phase == EXITING) {
				transition_groups.erase(it->key);
				node->queue_redraw(); // Redraw once more to avoid keeping the last frame
			}
		} else {
			transition_groups.erase(it->key);
		}
		++it;
	}
}

void StyleBox::begin_draw(RID p_canvas_item, const Rect2 &p_rect) const {
	current_draw_rect = Rect2(p_rect);
	setup_animation();

	const StyleBoxDrawModifier::Data &modifier = !cached_state
			? normal_modifier
			: cached_state->phase == EXITING
			? exiting_modifier
			: cached_state->phase == ENTERING
			? entering_modifier
			: normal_modifier;

	// Need to provide starting values for animations
	if (cached_state) {
		cached_state->has_drawn_box = true;
	}

	RenderingServer *vs = RenderingServer::get_singleton();
	Point2 animated_offset = get_animated_value("transform_offset", modifier.offset);
	Size2 animated_scale = get_animated_value("transform_scale", modifier.scale);
	real_t animated_rotation = get_animated_value("transform_rotation", modifier.rotation);
	Point2 pivot = get_animated_value("transform_pivot", p_rect.position + modifier.pivot * p_rect.size);
	Transform2D tf = Transform2D().translated(animated_offset) * Transform2D().translated(pivot) * Transform2D().rotated(animated_rotation) * Transform2D().scaled(animated_scale) * Transform2D().translated(-pivot);
	Color md = get_animated_value("modifier_modulate", modifier.modulate);

	total_modulate = Color(1, 1, 1, 1);
	for (Color &col : modulate_stack) {
		total_modulate *= col;
	}
	total_transform = Transform2D();
	for (Transform2D tr : transform_stack) {
		total_transform *= tr;
	}

	vs->canvas_item_add_set_transform(p_canvas_item, total_transform * tf);
	vs->canvas_item_add_set_modulate(p_canvas_item, total_modulate * md);
	if (cached_state) {
		cached_state->transform = tf;
		cached_state->modulate = md;
	}
}

void StyleBox::end_draw(RID p_canvas_item, const Rect2 &p_rect) const {
	RenderingServer *vs = RenderingServer::get_singleton();
	vs->canvas_item_add_set_modulate(p_canvas_item, total_modulate);
	vs->canvas_item_add_set_transform(p_canvas_item, total_transform);
}

void StyleBox::setup_animation() const {
	if (animation_id.is_empty()) {
		return;
	}

	Control *node = cast_to<Control>(get_current_item_drawn());
	if (!node) {
		return;
	}

	if (max_duration <= 0) {
		return;
	}

	AnimKey key = AnimKey(node->get_instance_id(), animation_id);
	AnimationState &current_state = transition_groups[key];
	cached_state = &current_state;
	current_state.current_time = OS::get_singleton()->get_ticks_usec();

	if (is_drawing_exit) {
		return;
	}
	if (is_drawing_enter) {
		return;
	}

	bool is_enterable = current_state.box.is_null() || current_state.phase == EXITING;
	current_state.box = this;
	current_state.rect = current_draw_rect;

	// Enter animations are decided here
	if (is_enterable && entering_mod_ref.is_valid()) {
		current_state.phase = ENTERING;
		bool cached_entering = is_drawing_enter;
		is_drawing_enter = true;
		draw(get_current_item_drawn()->get_canvas_item(), current_draw_rect);
		is_drawing_enter = cached_entering;
	} else {
		current_state.phase = NORMAL;
	}
}

Variant StyleBox::get_animated_value(StringName p_name, Variant p_default, StringName p_group, StringName p_info_prop) {
	Control *node = cast_to<Control>(CanvasItem::get_current_item_drawn());
	if (!node) {
		return p_default;
	}

	if (!p_group.is_empty()) {
		AnimKey key = AnimKey(node->get_instance_id(), p_group);
		cached_state = &transition_groups[key];
	}

	if (cached_state == nullptr) {
		return p_default;
	}

	AnimationState &current_state = *cached_state;
	AnimationValues &values = cached_state->values[p_name];

	if (cached_state->box.is_null()) {
		values.from = p_default;
		values.to = p_default;
		values.current = p_default;
		return p_default;
	}

	ObjectID nid = node->get_instance_id();
	Ref<StyleBox> info_box = (Ref<StyleBox>)cached_state->box;
	StringName info_key = p_info_prop.is_empty() ? p_name : p_info_prop;
	StyleBoxAnimationInfo::Data info;

	if (info_box->animation_infos.has(info_key)) {
		info = info_box->animation_infos[info_key];
	}
	if (info.duration < 0) {
		info.duration = info_box->animation_default.duration;
	}

	if (info.duration <= 0) {
		values.current = p_default;
		values.to = p_default;
		return p_default;
	}

	if (info.ease == Tween::EASE_MAX) {
		info.ease = info_box->animation_default.ease;
	}
	if (info.transition == Tween::TRANS_MAX) {
		info.transition = info_box->animation_default.transition;
	}

	uint64_t current_time = current_state.current_time;

	if (values.to != p_default) {
		values.from = values.current;
		values.to = p_default;
		values.start_time = current_time;
		current_state.end_time = MAX(current_state.end_time, current_time + info.duration * 1000000);
		if (!redrawer_connected) {
			node->get_tree()->connect("process_frame", callable_mp_static(StyleBox::handle_node_redraws));
			redrawer_connected = true;
		}
	}
	if (values.current == values.to) {
		return values.current;
	}
	if (values.from == Variant()) {
		values.from = p_default;
	}
	if (values.current == Variant()) {
		values.current = p_default;
	}
	double elapsed = (current_time - values.start_time) / 1000000.0;

	if (elapsed < info.duration) {
		Variant result = Animation::interpolate_variant(values.from, values.to, Tween::run_equation(info.transition, info.ease, elapsed, 0.0, 1.0, info.duration));
		values.current = result;
		return result;
	} else {
		values.current = values.to;
		values.from = values.to;
	}

	// For animations outside of a stylebox
	if (!p_group.is_empty()) {
		cached_state = nullptr;
	}

	return p_default;
}

void StyleBox::apply_group_modifiers(StringName p_id) {
	Control *node = cast_to<Control>(CanvasItem::get_current_item_drawn());
	RID item = node->get_canvas_item();
	if (!node) {
		return;
	}

	RenderingServer *vs = RenderingServer::get_singleton();

	ObjectID nid = node->get_instance_id();
	AnimKey key = AnimKey(nid, p_id);
	if (transition_groups.has(key)) {
		const AnimationState &state = transition_groups[key];
		if (state.box.is_valid() && state.box->normal_mod_ref.is_valid()) {
			transform_stack.push_back(state.transform);
			modulate_stack.push_back(state.modulate);
			total_transform *= state.transform;
			total_modulate *= state.modulate;
			vs->canvas_item_add_set_transform(item, total_transform);
			vs->canvas_item_add_set_modulate(item, total_modulate);
			return;
		}
	}
	transform_stack.push_back(Transform2D());
	modulate_stack.push_back(Color(1, 1, 1, 1));
}

void StyleBox::reset_modifiers() {
	Control *node = cast_to<Control>(CanvasItem::get_current_item_drawn());
	RID item = node->get_canvas_item();
	if (!node) {
		return;
	}

	RenderingServer *vs = RenderingServer::get_singleton();
	if (!transform_stack.is_empty()) {
		transform_stack.remove_at(transform_stack.size() - 1);
	}
	if (!modulate_stack.is_empty()) {
		modulate_stack.remove_at(modulate_stack.size() - 1);
	}
	total_modulate = Color(1, 1, 1, 1);
	for (Color &col : modulate_stack) {
		total_modulate *= col;
	}
	total_transform = Transform2D();
	for (Transform2D tr : transform_stack) {
		total_transform *= tr;
	}
	vs->canvas_item_add_set_modulate(item, total_modulate);
	vs->canvas_item_add_set_transform(item, total_transform);
}

void StyleBox::draw(RID p_canvas_item, const Rect2 &p_rect) const {
	GDVIRTUAL_CALL(_draw, p_canvas_item, p_rect);
}

Rect2 StyleBox::get_draw_rect(const Rect2 &p_rect) const {
	Rect2 ret;
	if (GDVIRTUAL_CALL(_get_draw_rect, p_rect, ret)) {
		return ret;
	}
	return p_rect;
}

CanvasItem *StyleBox::get_current_item_drawn() const {
	return CanvasItem::get_current_item_drawn();
}

bool StyleBox::test_mask(const Point2 &p_point, const Rect2 &p_rect) const {
	bool ret = true;
	GDVIRTUAL_CALL(_test_mask, p_point, p_rect, ret);
	return ret;
}

bool StyleBox::_set(const StringName &p_name, const Variant &p_value) {
	String full_property = String(p_name);

	if (!full_property.begins_with("animation/")) {
		return false;
	}

	Ref<StyleBoxAnimationInfo> info = p_value;
	String prop_name = full_property.trim_prefix("animation/");
	animation_info_refs[prop_name] = info;

	if (info.is_valid()) {
		if (p_name == SNAME("animation/default")) {
			animation_default.duration = MAX(0, info->get_duration());
			animation_default.ease = (Tween::EaseType)MIN(Tween::EASE_MAX - 1, info->get_ease());
			animation_default.transition = (Tween::TransitionType)MIN(Tween::TRANS_MAX - 1, info->get_transition());
		} else {
			StyleBoxAnimationInfo::Data &cached_info = animation_infos[prop_name];
			cached_info.duration = info->get_duration();
			cached_info.ease = info->get_ease();
			cached_info.transition = info->get_transition();
		}

		Callable updater = callable_mp(this, &StyleBox::_set).bind(p_name, p_value);
		if (!info->is_connected("changed", updater)) {
			info->connect("changed", updater);
		}
	} else {
		animation_infos.erase(prop_name);
	}

	max_duration = animation_default.duration;
	for (const KeyValue<StringName, StyleBoxAnimationInfo::Data> &kv : animation_infos) {
		max_duration = MAX(max_duration, kv.value.duration < 0 ? animation_default.duration : kv.value.duration);
	}

	emit_changed();
	return true;
}

bool StyleBox::_get(const StringName &p_name, Variant &r_ret) const {
	String full_property = String(p_name);

	if (!full_property.begins_with("animation/")) {
		return false;
	}

	String prop_name = full_property.trim_prefix("animation/");
	if (animation_info_refs.has(prop_name)) {
		r_ret = animation_info_refs[prop_name];
		return true;
	}
	return false;
}

void StyleBox::_get_property_list(List<PropertyInfo> *p_list) const {
	const LocalVector<StringName> &props = get_animatable_props();
	p_list->push_back(PropertyInfo(Variant::OBJECT, "animation/default", PROPERTY_HINT_RESOURCE_TYPE, "StyleBoxAnimationInfo"));
	p_list->push_back(PropertyInfo(Variant::NIL, "animation/overrides", PROPERTY_HINT_NONE, "animation/", PROPERTY_USAGE_SUBGROUP));
	for (auto &n : props) {
		p_list->push_back(PropertyInfo(Variant::OBJECT, "animation/" + String(n), PROPERTY_HINT_RESOURCE_TYPE, "StyleBoxAnimationInfo"));
	}
	p_list->push_back(PropertyInfo(Variant::OBJECT, "animation/normal_modifier", PROPERTY_HINT_RESOURCE_TYPE, "StyleBoxAnimationInfo"));
	p_list->push_back(PropertyInfo(Variant::OBJECT, "animation/enter_modifier", PROPERTY_HINT_RESOURCE_TYPE, "StyleBoxAnimationInfo"));
	p_list->push_back(PropertyInfo(Variant::OBJECT, "animation/exit_modifier", PROPERTY_HINT_RESOURCE_TYPE, "StyleBoxAnimationInfo"));
}

void StyleBox::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_minimum_size"), &StyleBox::get_minimum_size);

	ClassDB::bind_method(D_METHOD("set_content_margin", "margin", "offset"), &StyleBox::set_content_margin);
	ClassDB::bind_method(D_METHOD("set_content_margin_all", "offset"), &StyleBox::set_content_margin_all);
	ClassDB::bind_method(D_METHOD("get_content_margin", "margin"), &StyleBox::get_content_margin);

	ClassDB::bind_method(D_METHOD("get_margin", "margin"), &StyleBox::get_margin);
	ClassDB::bind_method(D_METHOD("get_offset"), &StyleBox::get_offset);

	ClassDB::bind_method(D_METHOD("set_draw_modifier", "phase", "draw_modifier"), &StyleBox::set_draw_modifier);
	ClassDB::bind_method(D_METHOD("get_draw_modifier", "phase"), &StyleBox::get_draw_modifier);

	ClassDB::bind_method(D_METHOD("draw", "canvas_item", "rect"), &StyleBox::draw);
	ClassDB::bind_method(D_METHOD("get_current_item_drawn"), &StyleBox::get_current_item_drawn);

	ClassDB::bind_method(D_METHOD("test_mask", "point", "rect"), &StyleBox::test_mask);

	ADD_GROUP("Content Margins", "content_margin_");
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "content_margin_left", PROPERTY_HINT_RANGE, "-1,2048,1,suffix:px"), "set_content_margin", "get_content_margin", SIDE_LEFT);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "content_margin_top", PROPERTY_HINT_RANGE, "-1,2048,1,suffix:px"), "set_content_margin", "get_content_margin", SIDE_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "content_margin_right", PROPERTY_HINT_RANGE, "-1,2048,1,suffix:px"), "set_content_margin", "get_content_margin", SIDE_RIGHT);
	ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "content_margin_bottom", PROPERTY_HINT_RANGE, "-1,2048,1,suffix:px"), "set_content_margin", "get_content_margin", SIDE_BOTTOM);

	ADD_GROUP("Draw Modifiers", "modifier_");
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "modifier_normal", PROPERTY_HINT_RESOURCE_TYPE, "StyleBoxDrawModifier"), "set_draw_modifier", "get_draw_modifier", NORMAL);
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "modifier_entering", PROPERTY_HINT_RESOURCE_TYPE, "StyleBoxDrawModifier"), "set_draw_modifier", "get_draw_modifier", ENTERING);
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "modifier_exiting", PROPERTY_HINT_RESOURCE_TYPE, "StyleBoxDrawModifier"), "set_draw_modifier", "get_draw_modifier", EXITING);

	GDVIRTUAL_BIND(_draw, "to_canvas_item", "rect")
	GDVIRTUAL_BIND(_get_draw_rect, "rect")
	GDVIRTUAL_BIND(_get_minimum_size)
	GDVIRTUAL_BIND(_test_mask, "point", "rect")
}

StyleBox::StyleBox() {
	for (int i = 0; i < 4; i++) {
		content_margin[i] = -1;
	}
}
