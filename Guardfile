guard :shell do
  watch /(spec\.lua$)/ do |m|
    if system("busted #{m[0]}")
      n "#{m[0]} passes", 'lua (busted)', :success
    else
      n "#{m[0]} fails", 'lua (busted)', :failed
    end
  end
end
