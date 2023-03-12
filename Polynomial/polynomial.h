#ifndef __POLYNOMIAL_LAB_
#define __POLYNOMIAL_LAB_
#include <iostream>
#include <string>

class Polynomial {
protected:
	struct Monomial {
		static int convert_to_power(int p1, int p2, int p3);
		static void convert_back(int power, int& p1, int& p2, int& p3);
		double coef;
		int power;
		Monomial* next;
		Monomial(double _coef, int _power, Monomial* _next = nullptr) : coef{ _coef }, power{ _power }, next{ _next } {}
		Monomial(const Monomial& m) {
			coef = m.coef;
			power = m.power;
			if (m.next)
				next = new Monomial(*m.next);
			else
				next = nullptr;
		}
		~Monomial() {
			delete next;
		}
	};
protected:
	static constexpr int min_power = -10;
	static constexpr int max_power = 10;
	static constexpr int number_of_diff_powers = max_power - min_power + 1;
	void add_last(double coef, int power);
	void add_last(double coef, int p1, int p2, int p3) {
		if (min_power <= p1 && p1 <= max_power &&
			min_power <= p2 && p2 <= max_power &&
			min_power <= p3 && p3 <= max_power)
			add_last(coef, Monomial::convert_to_power(p1, p2, p3));
	}
	void add(double coef, int power);
protected:
	Monomial* start;
	Monomial* last;
public:
	Polynomial() {
		last = start = nullptr;
	}
	Polynomial(const Polynomial& p) {
		if (p.start == nullptr) {
			last = start = nullptr;
			return;
		}
		start = new Monomial(*p.start);
		last = start;
		while (last->next != nullptr)
			last = last->next;
	}
	void clear() {
		delete start;
		last = start = nullptr;
	}
	~Polynomial() {
		clear();
	}
	void delete_zero_monomials();
	void add(double coef, int p1, int p2, int p3) {
		if (min_power <= p1 && p1 <= max_power &&
			min_power <= p2 && p2 <= max_power &&
			min_power <= p3 && p3 <= max_power)
			add(coef, Monomial::convert_to_power(p1, p2, p3));
	}
	double calculate(double x, double y, double z) const;
	Polynomial& operator=(const Polynomial& p);
	Polynomial operator+() const;
	Polynomial operator-() const;
	Polynomial operator+(const Polynomial& p) const;
	Polynomial operator-(const Polynomial& p) const;
	Polynomial operator*(const Polynomial& p) const;
	friend std::istream& operator>>(std::istream& in, Polynomial& p);
	friend std::ostream& operator<<(std::ostream& out, const Polynomial& p);
};

#endif // !__POLYNOMIAL_LAB_
