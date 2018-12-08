require 'test/unit'
require_relative 'fib-sequence'

class FibonacciSequenceTest < Test::Unit::TestCase
  def setup
    @mySeq = FibonacciSequence.new
  end

  # test metody reset
  def test_reset
    assert_equal( true, @mySeq.reset )
  end

  # test metody next
  def test_next
    @mySeq.reset
    assert_equal( 0, @mySeq.next)
    assert_equal( 1, @mySeq.next)
  end

  # test metody current
  def test_current
   @mySeq.reset
   assert_equal( nil, @mySeq.current )
   @mySeq.next
   assert_equal( 0, @mySeq.current )
  end
  
  # test metody current_idx
  def test_current_idx
   @mySeq.reset
   assert_equal( nil, @mySeq.current_idx )
   @mySeq.next
   assert_equal( 0, @mySeq.current_idx )  
  end
  # test metody []
  def test_funct 
   @mySeq.reset
   assert_equal( nil, @mySeq.[](-1) )
   @mySeq.next
   assert_equal( 0, @mySeq.[](0) )
   assert_equal( 1, @mySeq.[](1) )
  end  
end
