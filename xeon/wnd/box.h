#pragma once

class Box {
public:
	Box() = default;
	Box(int l, int t, int r, int b) : 
		left(l), top(t), right(r), bottom(b) {}
	~Box() = default;
	
	int left, top, right, bottom;
};

