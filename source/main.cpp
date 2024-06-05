#include <SFML/Graphics.hpp>

#include <random>
#include <thread>

class Field {
public:
	Field() {
		init_field(m_field);
		init_field(m_next_generation);
	}

	void init_field(std::vector<std::vector<int>>& field) {
		field.resize(m_rows_num);
		for (auto& row : field) {
			row.resize(m_cols_num);
			for (auto& el : row)
				el = std::rand() % 100 < 20;
		}
	}

	void logic() {
		m_next_generation = m_field;
		for (size_t i = 0; i < m_rows_num; ++i)
			for (size_t j = 0; j < m_cols_num; ++j)
				process_cell(i, j);
		m_field = m_next_generation;

		if (std::all_of(m_field.begin(), m_field.end(), [](auto& row) {
			return std::all_of(row.begin(), row.end(), [](auto& el) {return el == 0; });
		})) { // поколение вымерло => перезапуск
			for (auto& row : m_field) {
				for (auto& el : row)
					el = std::rand() % 100 < 20;
			}
		}
	}

	int get_value(int i, int j) { //Todo
		if (i < 0)
			i = m_rows_num - 1;
		if (j < 0)
			j = m_cols_num - 1;
		if (i >= m_rows_num)
			i = 0;
		if (j >= m_cols_num)
			j = 0;

		return m_field[i][j];
	}

	void process_cell(size_t i, size_t j) {
		size_t alive_neights = 0;
		for (auto& [dx, dy] : neights) {
			if (get_value(i + dx, j + dy))
				++alive_neights;
		}
		if (get_value(i, j) == 0) { // мертвая клетка
			if (alive_neights == 3)
				m_next_generation[i][j] = 1;
		}
		else {
			if (alive_neights < 2 || alive_neights > 3)
				m_next_generation[i][j] = 0;
		}
	}

	void draw(sf::RenderWindow& window) {
		auto win_size = window.getSize();
		double cell_size = std::min(win_size.x, win_size.y) / std::max(m_cols_num, m_rows_num);

		double x_align = (win_size.x - cell_size * m_rows_num) / 2;
		double y_align = (win_size.y - cell_size * m_cols_num) / 2;

		static sf::Vertex vert[2];
		for (size_t i = 0; i <= m_rows_num; ++i) {
			vert[0] = sf::Vector2f(0, cell_size * i);
			vert[1] = sf::Vector2f(cell_size * m_cols_num, cell_size * i);
			vert[0].position += sf::Vector2f(x_align, y_align);
			vert[1].position += sf::Vector2f(x_align, y_align);
			window.draw(vert, 2, sf::PrimitiveType::Lines);
		}
		for (size_t j = 0; j <= m_cols_num; ++j) {
			vert[0] = sf::Vector2f(cell_size * j, 0);
			vert[1] = sf::Vector2f(cell_size * j, cell_size * m_rows_num);
			vert[0].position += sf::Vector2f(x_align, y_align);
			vert[1].position += sf::Vector2f(x_align, y_align);
			window.draw(vert, 2, sf::PrimitiveType::Lines);
		}

		for (size_t i = 0; i < m_rows_num; ++i) {
			for (size_t j = 0; j < m_cols_num; ++j) {
				if (m_field[i][j]) {
					double r = cell_size / 2.;
					sf::CircleShape circle(cell_size / 2.);
					//circle.setOrigin(-r, -r);

					circle.setPosition(sf::Vector2f(i * cell_size, j * cell_size) + sf::Vector2f(x_align, y_align));

					window.draw(circle);
				}
			}
		}

	}

private:

	static std::vector<std::pair<int, int>> neights;

	size_t m_cols_num = 30, m_rows_num = 30;

	std::vector<std::vector<int>> m_field;
	std::vector<std::vector<int>> m_next_generation;
 
};


std::vector<std::pair<int, int>> Field::neights = {
		{1,0},
		{0,1},
		{1,1},
		{-1,0},
		{-1,1},
		{0, -1},
		{1, -1},
		{-1,-1}
};

int main() {
	sf::Vector2f win_size = { 1200, 600 };
	sf::RenderWindow window(sf::VideoMode(win_size.x, win_size.y), "VHelps");
	Field field;


	while (window.isOpen()) {
		sf::Event event;
		
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				return 0;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		field.logic();
		window.clear();
		field.draw(window);
		window.display();
	}



	return 0;
}