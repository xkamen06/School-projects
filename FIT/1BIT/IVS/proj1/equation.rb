class Equation 
  def self.solve_quadratic(a, b, c)
	# a i b je rovno 0
	if ((a == 0) && (b == 0))
		return nil
	end
	# a je rovno 0
	if (a == 0) 
		return [-c/b.to_f]
	end
	# vypocet diskriminantu
	@diskrim = (b**2) - (4*a*c)
        # diskriminant je roven 0
	if (@diskrim == 0)
		@vysledek = [(-b)/(2*a)]
		return @vysledek
	# diskriminant je kladne realne cislo
	elsif (@diskrim > 0)
		@vysl_1 = (-b + Math.sqrt(@diskrim))/(2*a)
		@vysl_2 = (-b - Math.sqrt(@diskrim))/(2*a)
		@vysledek = [@vysl_1, @vysl_2]
		return @vysledek
	# diskriminant je zaporny
	else
		return nil
	end
  end
end
