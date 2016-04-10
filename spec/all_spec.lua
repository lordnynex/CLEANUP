local Escambo, string, table =
  require [[..escambo]], require [[string]], require [[table]]

local format, concat, escambo = string.format, table.concat, Escambo()

local charsets_tests = {
  {nil, {'utf-8'}, {'utf-8'}}, {'utf-8', {'utf-8'}, {'utf-8'}},
  {'*', {'utf-8'}, {'utf-8'}}, {'utf-8', {'utf-8', 'ISO-8859-1'}, {'utf-8'}},
  {'utf-8, ISO-8859-1', {'utf-8'}, {'utf-8'}},
  {'utf-8;q=0.8, ISO-8859-1', nil, {'ISO-8859-1', 'utf-8'}},
  {'*, utf-8;q=0', {'utf-8', 'ISO-8859-1'}, {'ISO-8859-1'}},
  {'utf-8;q=0.8, ISO-8859-1', {'utf-8', 'ISO-8859-1'}, {'ISO-8859-1', 'utf-8'}},
  {'*, utf-8', {'utf-8', 'ISO-8859-1'}, {'utf-8', 'ISO-8859-1'}}
}

describe('charset tests', function()

  local charsets

  teardown(function()
    charsets = nil
  end)

  setup(function()
    charsets = escambo.charsets
  end)

  it('should not return a charset when no charset is provided', function()
    assert.are.same(charsets(escambo, {}, '*'), {})
  end)

  it('should not return a charset when no charset is acceptable', function()
    assert.are.same(charsets(escambo, {'utf-8'}, 'ISO-8859-1'), {})
  end)

  it('should not return a charset with q = 0', function()
    assert.are.same(charsets(escambo, {'utf-8'}, 'utf-8;q=0'), {})
  end)

  for i = 1, #charsets_tests do
    it(
      format(
        '#%d - should return `%s` for accept-charset `%s` with provided charset `%s`',
        i, (charsets_tests[i][3] and concat(charsets_tests[i][3], ', ')
          or tostring(charsets_tests[i][3])),
        tostring(charsets_tests[i][1]),
        (charsets_tests[i][2] and concat(charsets_tests[i][2], ', ')
          or tostring(charsets_tests[i][2]))
      ),
      function ()
        assert.are.same(
          charsets(escambo, charsets_tests[i][2], charsets_tests[i][1]), charsets_tests[i][3]
        )
    end)
  end

end)

local encodings_conf = {
  {nil, {'identity', 'gzip'}, {'identity'}},
  {'gzip', {'identity', 'gzip'}, {'gzip', 'identity'}},
  {'gzip, compress', {'compress'}, {'compress'}},
  {'deflate', {'gzip', 'identity'}, {'identity'}},
  {'*', {'identity', 'gzip'}, {'identity', 'gzip'}},
  {'gzip, compress', {'compress', 'identity'}, {'compress', 'identity'}},
  {'gzip;q=0.8, compress', {'gzip', 'compress'}, {'compress', 'gzip'}},
  {'*, compress;q=0', {'gzip', 'compress'}, {'gzip'}},
  {'gzip;q=0.8, compress', nil, {'compress', 'gzip', 'identity'}},
  {'*, compress', {'gzip', 'compress'}, {'compress', 'gzip'}},
  {'gzip, compress;q=0', {'compress', 'identity'}, {'identity'}},
  {
    'gzip;q=0.8, identity;q=0.5, *;q=0.3',
    {'identity', 'gzip', 'compress'},
    {'gzip', 'identity', 'compress'}
  }
}

describe('encoding tests', function()

  local encodings

  teardown(function()
    encodings = nil
  end)

  setup(function()
    encodings = escambo.encodings
  end)

  it('should return identity encoding when no encoding is provided', function()
    assert.are.same(encodings(escambo, {}, nil), {'identity'})
  end)

  it('should include the identity encoding even if not explicity listed', function()
    assert.are.same(encodings(escambo, nil, 'gzip'), {'gzip', 'identity'})
  end)

  it('should not return identity encoding if q = 0', function()
    assert.are.same(encodings(escambo, nil, 'identity;q=0'), {})
  end)

  it('should not return identity encoding if * has q = 0', function()
    assert.are.same(encodings(escambo, nil, '*;q=0'), {})
  end)

  it(
    'should not return identity encoding if * has q = 0 but identity explicitly has q > 0',
    function()
    assert.are.same(encodings(escambo, nil, '*;q=0,identity;q=0.5'), {'identity'})
  end)

  for i = 1, #encodings_conf do
    it(
      format(
        '#%d - should return `%s` for accept-encoding `%s` with provided encoding `%s`',
        i, (encodings_conf[i][3] and concat(encodings_conf[i][3], ', ')
          or tostring(encodings_conf[i][3])),
        tostring(encodings_conf[i][1]),
        (encodings_conf[i][2] and concat(encodings_conf[i][2], ', ')
          or tostring(encodings_conf[i][2]))
      ),
      function ()
        assert.are.same(
          encodings(escambo, encodings_conf[i][2], encodings_conf[i][1]), encodings_conf[i][3]
        )
    end)
  end

end)

local languages_conf = {
  {nil, {'en'}, {'en'}}, {'en', {'en'}, {'en'}}, {'*', {'en'}, {'en'}},
  {'en-US, en;q=0.8', {'en-US', 'en-GB'}, {'en-US', 'en-GB'}},
  {'en-US, en-GB', {'en-US'}, {'en-US'}}, {'en', {'en', ''}, {'en'}},
  {'en', {'en-US'}, {'en-US'}}, {'en;q=0.8, es', {'en', 'es'}, {'es', 'en'}},
  {'en-US;q=0.8, es', {'en', 'es'}, {'es', 'en'}}, {'*, en;q=0', {'en', 'es'}, {'es'}},
  {'en-US;q=0.8, es', nil, {'es', 'en-US'}}, {'*, en', {'es', 'en'}, {'en', 'es'}},
  {
    'nl;q=0.5,fr,de,en,it,es,pt,no,se,fi,ro',
    {'fr', 'de', 'en', 'it', 'es', 'pt', 'no', 'se', 'fi', 'ro', 'nl'},
    {'fr', 'de', 'en', 'it', 'es', 'pt', 'no', 'se', 'fi', 'ro', 'nl'}
  }
}

describe('language tests', function()

  local languages

  teardown(function()
    languages = nil
  end)

  setup(function()
    languages = escambo.languages
  end)

  it('should not return a language when no is provided', function ()
    assert.are.same(languages(escambo, {}, '*'), {})
  end)

  it('should not return a language when no language is acceptable', function ()
    assert.are.same(languages(escambo, {'es'}, 'en'), {})
  end)

  it('should not return a language with q = 0', function()
    assert.are.same(languages(escambo, {'en'}, 'en;q=0'), {})
  end)

  for i = 1, #languages_conf do
    it(
      format(
        '#%d - should return `%s` for accept-language `%s` with provided language `%s`',
        i, (languages_conf[i][3] and concat(languages_conf[i][3], ', ')
          or tostring(languages_conf[i][3])),
        tostring(languages_conf[i][1]),
        (languages_conf[i][2] and concat(languages_conf[i][2], ', ')
          or tostring(languages_conf[i][2]))
      ),
      function ()
        assert.are.same(
          languages(escambo, languages_conf[i][2], languages_conf[i][1]), languages_conf[i][3]
        )
    end)
  end

end)

local media_conf = {
  {nil, {'text/html'}, {'text/html'}}, {'text/html', {'text/html'}, {'text/html'}},
  {'text/html;level', {'text/html'}, {'text/html'}},
  {'*/*', {'text/html'}, {'text/html'}}, {'text/*', {'text/html'}, {'text/html'}},
  {'application/json, text/html', {'text/html'}, {'text/html'}},
  {'text/html;q=0.1', {'text/html'}, {'text/html'}}, {
    'application/json, text/html',
    {'application/json', 'text/html'},
    {'application/json', 'text/html'}
  }, {
    'application/json;q=0.2, text/html',
    {'application/json', 'text/html'},
    {'text/html', 'application/json'}
  }, {'application/json;q=0.2, text/html', nil, {'text/html', 'application/json'}},
  {'text/*, text/html;q=0', {'text/html', 'text/plain'}, {'text/plain'}},
  {'text/*, text/html;q=0.5', {'text/html', 'text/plain'}, {'text/plain', 'text/html'}},
  {
    'application/json, */*; q=0.01',
    {'text/html', 'application/json'},
    {'application/json', 'text/html'}
  }, {
    'application/vnd.example;attribute=value',
    {'application/vnd.example;attribute=other', 'application/vnd.example;attribute=value'},
    {'application/vnd.example;attribute=value'}
  }, {
    'application/vnd.example;attribute=other',
    {'application/vnd.example', 'application/vnd.example;attribute=other'},
    {'application/vnd.example;attribute=other'}
  }, {'text/html;level=1', {'text/html;level=1;foo=bar'}, {'text/html;level=1;foo=bar'}},
  {'text/html;level=1;foo=bar', {'text/html;level=1'}, {}},
  {'text/html;level=2', {'text/html;level=1'}, {}},
  {
    'text/html, text/html;level=1;q=0.1',
    {'text/html', 'text/html;level=1'},
    {'text/html', 'text/html;level=1'}
  }, {
    'text/*;q=0.3, text/html;q=0.7, text/html;level=1, text/html;level=2;q=0.4, */*;q=0.5',
    {'text/html;level=1', 'text/html', 'text/html;level=3', 'image/jpeg', 'text/html;level=2', 'text/plain'},
    {'text/html;level=1', 'text/html', 'image/jpeg', 'text/html;level=2', 'text/plain', 'text/html;level=3'}
  }, {
    'text/html, application/xhtml+xml, */*', {'application/json', 'text/html'},
    {'text/html', 'application/json'}
  }
}

describe('media type tests', function()

  local mediatypes

  teardown(function()
    mediatypes = nil
  end)

  setup(function()
    mediatypes = escambo.media_types
  end)

  it('should not return a media type when no media type provided', function()
    assert.are.same(mediatypes(escambo, {}, '*/*'), {})
  end)

  it('should not return a media type when no media type is acceptable', function()
    assert.are.same(mediatypes(escambo, {'text/html'}, 'application/json'), {})
  end)

  it('should not return a media type with q = 0', function()
    assert.are.same(mediatypes(escambo, {'text/html'}, 'text/html;q=0'), {})
  end)

  it('should handle extra slashes on query params', function()
    assert.are.same(
      mediatypes(
        escambo,
        {'application/xhtml+xml;profile="http://www.wapforum.org/xhtml"'},
        'application/xhtml+xml;profile="http://www.wapforum.org/xhtml"'
      ), 
      {'application/xhtml+xml;profile="http://www.wapforum.org/xhtml"'}
    )
  end)

  for i = 1, #media_conf do
    it(
      format(
        '#%d - should return `%s` for accept `%s` with provided media type `%s`',
        i, (media_conf[i][3] and concat(media_conf[i][3], ', ')
          or tostring(media_conf[i][3])),
        tostring(media_conf[i][1]),
        (media_conf[i][2] and concat(media_conf[i][2], ', ')
          or tostring(media_conf[i][2]))
      ),
      function ()
        assert.are.same(
          mediatypes(escambo, media_conf[i][2], media_conf[i][1]), media_conf[i][3]
        )
    end)
  end

end)