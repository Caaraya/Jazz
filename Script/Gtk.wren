foreign class Button {
	foreign construct new(label)
	foreign construct from(other)
	foreign show()
	foreign show_all()
	foreign hide()
}
foreign class Window {
	foreign construct new(type)
	foreign show()
	foreign show_all()
	foreign set_default_size(w, h)
	foreign add(child)
	foreign remove(child)
	foreign hide()
	foreign get_child()
}