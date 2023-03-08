#ifndef __POLYNOMIAL_LAB_
#define __POLYNOMIAL_LAB_
#include <iostream>

class Polynomial {
private:
	struct Monomial {
		int power;
		int coef;
		Monomial* next;
		Monomial(int _power, int _coef, Monomial* _next = nullptr) : power{ _power }, coef{ _coef }, next{ _next } {}
		Monomial(const Monomial& m) {
			power = m.power;
			coef = m.coef;
			if (m.next)
				next = new Monomial(*m.next);
			else
				next = nullptr;
		}
		~Monomial() {
			delete next;
		}
	};
	Monomial* start;
	Monomial* last;
	void add_last();
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
	void add_monomial();
	void delete_zero_monomials();
	Polynomial operator+();
	Polynomial operator+();
	Polynomial operator+(const Polynomial& p);
	Polynomial operator-(const Polynomial& p);
	Polynomial operator*(const Polynomial& p);
	friend std::istream& operator>>(std::istream& in, Polynomial& p);
	friend std::ostream& operator<<(std::ostream& out, const Polynomial& p);
};

#endif // !__POLYNOMIAL_LAB_
