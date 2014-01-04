int main() {
	int i = 1;
	char c = '2';
	[&i, c]() {
		i = 2;
	};
	return 0;
}