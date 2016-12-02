data = []

(10..1000).each { |i|
  data << "R#{i}"
}

(10..1000).each { |i|
  data << "L#{i}"
}

puts data.join(", ")
