#include <cmath>
#include "polynomial.h"

int Polynomial::convert_to_power(int p1, int p2, int p3) {
	p1 += 10; p2 += 10; p3 += 10;
	return 21 * 21 * p1 + 21 * p2 + p3;
}

void Polynomial::convert_back(int power, int& p1, int& p2, int& p3) {
	p3 = power % 21 - 10;
	power /= 21;
	p2 = power % 21 - 10;
	power /= 21;
	p1 = power % 21 - 10;
}

void Polynomial::add_last(double coef, int power) {
	if (start == nullptr) {
		start = new Monomial(coef, power);
		last = start;
	}
	else {
		last->next = new Monomial(coef, power);
		last = last->next;
	}
}

void Polynomial::add_monomial(double coef, int power) {
	if (start == nullptr) {
		add_last(coef, power);
		return;
	}
	if (start->power == power) {
		start->coef += coef;
		return;
	}
	if (start->power > power) {
		start = new Monomial(coef, power, start);
		return;
	}
	Monomial* now = start;
	while (now->next != nullptr) {
		if (now->next->power == power) {
			now->next->coef += coef;
			return;
		}
		if (now->next->power > power) {
			now->next = new Monomial(coef, power, now->next);
			return;
		}
	}
	add_last(coef, power);
}

void Polynomial::delete_zero_monomials() {
	while (start != nullptr && start->coef == 0.0) {
		auto temp = start->next;
		start->next == nullptr;
		delete start;
		start = temp;
	}
	if (start == nullptr) {
		last = nullptr;
		return;
	}
	Monomial* now = start;
	while (now->next != nullptr) {
		while (now->next != nullptr && now->next->coef == 0.0) {
			auto temp = now->next->next;
			now->next->next = nullptr;
			delete now->next;
			now->next = temp;
		}
		if (now->next != nullptr)
			now = now->next;
	}
	last = now;
}

double Polynomial::calculate(double x, double y, double z) const {
	double ans = 0.0;
	Monomial* now = start;
	while (now != nullptr) {
		int p[3]{};
		convert_back(now->power, p[0], p[1], p[2]);
		ans += now->coef * std::pow(x, p[0]) * std::pow(y, p[1]) * std::pow(z, p[2]);
		now = now->next;
	}
	return ans;
}

Polynomial Polynomial::operator+() const {
	return *this;
}

Polynomial Polynomial::operator-() const {
	Polynomial ret = Polynomial(*this);
	auto now = ret.start;
	while (now != nullptr) {
		now->coef = -now->coef;
		now = now->next;
	}
	return ret;
}

Polynomial Polynomial::operator+(const Polynomial& p) const {
	Polynomial ret;
	Monomial* now1 = this->start;
	Monomial* now2 = p.start;
	while(now1 != nullptr && now2 != nullptr) {
		if (now1->power == now2->power) {
			ret.add_last(now1->coef + now2->coef, now1->power);
			now1 = now1->next;
			now2 = now2->next;
		}
		else if (now1->power < now2->power) {
			ret.add_last(now1->coef, now1->power);
			now1 = now1->next;
		}
		else {
			ret.add_last(now2->coef, now2->power);
			now2 = now2->next;
		}
	}
	while (now1 != nullptr) {
		ret.add_last(now1->coef, now1->power);
		now1 = now1->next;
	}
	while (now2 != nullptr) {
		ret.add_last(now2->coef, now2->power);
		now2 = now2->next;
	}
	ret.delete_zero_monomials();
	return ret;
}

Polynomial Polynomial::operator-(const Polynomial& p) const {
	return *this + -p;
}

Polynomial Polynomial::operator*(const Polynomial& p) const {
	Polynomial ret;
	Monomial* now1 = this->start;
	while (now1 != nullptr)	{
		Monomial* now2 = p.start;
		while (now2 != nullptr)	{
			int r1[3]{}, r2[3]{};
			convert_back(now1->power, r1[0], r1[1], r1[2]);
			convert_back(now2->power, r2[0], r2[1], r2[2]);
			ret.add_monomial(now1->coef * now2->coef, r1[0] + r2[0], r1[1] + r2[1], r1[2] + r2[2]);
			now2 = now2->next;
		}
		now1 = now1->next;
	}
	ret.delete_zero_monomials();
	return ret;
}

std::istream& operator>>(std::istream& in, Polynomial& p) {
	return in;
}

std::ostream& operator<<(std::ostream& out, const Polynomial& p) {
	return out;
}
