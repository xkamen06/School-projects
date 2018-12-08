class FibonacciSequence 
 
  def initialize( f0 = 0, f1 = 1 )
    @seq = [f0, f1]
    self.reset
  end
  
  # nastavi radu do vychoziho stavu 
  def reset
    @idx = -1
    
    return true
  end
  
  # vrati dalsi fibonacciho cislo
  def next
    @idx += 1   
    if ( @idx == 0) 
      @seq[@idx] = 0
      return @seq[@idx]
    end
    if ( @idx == 1) 
      @seq[@idx] = 1
      return @seq[@idx]
    end
    if ( @idx > 1 ) 
      @val_1 = 0
      @val_2 = 1
      @res = 0
      @a = 0
      while (@a + 1) < @idx do
        @res = @val_1 + @val_2
        @val_1 = @val_2
	@val_2 = @res
	@a = @a + 1
      end
      @seq[@idx] = @res  	
      return @res
    end
  end
  
  # aktualni fibonacciho cislo
  def current
    return @idx >= 0 ? @seq[@idx] : nil
  end
  
  # aktualni index (vraci nil, pokud se jeste nezacalo generovat)
  def current_idx
    return @idx >= 0 ? @idx : nil
  end
  
  # vrati fibonacciho cislo s danym indexem
  def [](n)
    return nil if n < 0
    return @seq[n] if n <= @idx
    
    while @idx < n
      self.next
    end
    
    return self.current
  end
end

my=FibonacciSequence.new
my.[](1000)
