
require [[spec.helper]]

local format = require('string').format

describe('should parse', function()

  local escambo, invalid

  setup(function()
    escambo = require('..escambo')()
    invalid = {
      ' ', 'null', 'undefined', '/', 'text / plain', 'text/;plain', 'text/"plain"',
      'text/p£ain', 'text/(plain)', 'text/@plain', 'text/plain,wrong'
    }
  end)

  teardown(function()
    escambo, invalid = nil, nil
  end)

  describe('escambo.parse(string)', function()

    it('should parse basic type', function ()
      assert.safesame({type='text/html'}, escambo.parse('text/html'))
      assert.safesame({type='text', subtype='html'}, escambo.parse('text/html', true))
    end)

    it('should parse with suffix', function()
      assert.safesame({type='image/svg+xml'}, escambo.parse('image/svg+xml'))
      assert.safesame({type='image', subtype='svg', suffix='xml'}, escambo.parse('image/svg+xml', true))
    end)

    it('should parse basic type with surrounding OWS', function()
      assert.safesame({type='text/html'}, escambo.parse(' text/html '))
      assert.safesame({type='text', subtype='html'}, escambo.parse(' text/html ', true))
    end)

    it('should parse parameters', function()
      assert.safesame(
        {type='text/html', parameters={charset='utf-8', foo='bar'}},
        escambo.parse('text/html; charset=utf-8; foo=bar')
      )
      assert.safesame(
        {type='text', subtype='html', parameters={charset='utf-8', foo='bar'}},
        escambo.parse('text/html; charset=utf-8; foo=bar', true)
      )
    end)

    it('should parse parameters with extra LWS', function()
      assert.safesame(
        {type='text/html', parameters={charset='utf-8', foo='bar'}},
        escambo.parse('text/html ; charset=utf-8 ; foo=bar')
      )
      assert.safesame(
        {type='text', subtype='html', parameters={charset='utf-8', foo='bar'}},
        escambo.parse('text/html ; charset=utf-8 ; foo=bar', true)
      )
    end)

    it('should lower-case type', function()
      assert.safesame({type='image/svg+xml'}, escambo.parse('IMAGE/SVG+XML'))
      assert.safesame({type='image', subtype='svg', suffix='xml'}, escambo.parse('IMAGE/SVG+XML', true))
    end)

    it('should lower-case parameter names', function()
      assert.safesame(
        {type='text/html', parameters={charset='UTF-8'}},
        escambo.parse('text/html; Charset=UTF-8')
      )
      assert.safesame(
        {type='text', subtype='html', parameters={charset='UTF-8'}},
        escambo.parse('text/html; Charset=UTF-8', true)
      )
    end)

    it('should unquote parameter values', function()
      assert.safesame(
        {type='text/html', parameters={charset='UTF-8'}}, escambo.parse('text/html; charset="UTF-8"')
      )
      assert.safesame(
        {type='text', subtype='html', parameters={charset='UTF-8'}},
        escambo.parse('text/html; charset="UTF-8"', true)
      )
    end)

    it('should unquote parameter values with escapes', function()
      assert.safesame(
        {type='text/html', parameters={charset='UTF-\\"8"'}},
        escambo.parse('text/html; charset = "UT\\F-\\\\\\"8\\""')
      )
      assert.safesame(
        {type='text', subtype='html', parameters={charset='UTF-\\"8"'}},
        escambo.parse('text/html; charset = "UT\\F-\\\\\\"8\\""', true)
      )
    end)

    it('should handle balanced quotes', function()
      assert.safesame(
        {type='text/html', parameters={param='charset="utf-8"; foo=bar', bar='foo'}},
        escambo.parse('text/html; param="charset=\\"utf-8\\"; foo=bar"; bar=foo')
      )
      assert.safesame(
        {type='text', subtype='html', parameters={param='charset="utf-8"; foo=bar', bar='foo'}},
        escambo.parse('text/html; param="charset=\\"utf-8\\"; foo=bar"; bar=foo', true)
      )
    end)

    it('should throw on invalid parameter format #puto', function()
      assert.safefail('invalid parameter format', escambo.parse('text/plain; foo="bar'))
      assert.safefail('invalid parameter format', escambo.parse('text/plain; profile=http://localhost'))
      assert.safefail(
        'invalid parameter format',
        escambo.parse('text/plain; profile=http://localhost; foo=bar')
      )
      assert.safefail(
        'invalid parameter format',
        escambo.parse('text/plain; foo="bar"; profile=http://localhost')
      )
    end)

    it('should require argument', function()
      assert.safefail('argument string is required', escambo.parse())
    end)

    it('should reject non-strings', function()
      assert.safefail('argument string is required to be a string', escambo.parse(7))
    end)

    for i=1, #invalid do
      it(format('should fail on invalid media type `%s` #%d', invalid[i], i), function()
        assert.safefail('invalid media type', escambo.parse(invalid[i]))
      end)
    end

  end)

  describe('escambo.parse(req)', function()

    it('should parse content-type header', function()
      local req = {headers={['content-type']='text/html'}}
      assert.safesame({type='text/html'}, escambo.parse(req))
      assert.safesame({type='text', subtype='html'}, escambo.parse(req, true))
    end)

    it('should reject objects without headers property', function()
      assert.safefail('argument string is required to be a string', escambo.parse({}))
    end)

    it('should reject missing content-type', function()
      local req = {headers={}}
      assert.safefail('argument string is required to be a string', escambo.parse(req))
    end)

  end)

  describe('escambo.parse(res)', function()

    it('should parse content-type header', function()
      local res = {get_header = function() return 'text/html' end}
      assert.safesame({type='text/html'}, escambo.parse(res))
      assert.safesame({type='text', subtype='html'}, escambo.parse(res, true))
    end)

    it('should reject objects without getHeader method', function()
      assert.safefail('argument string is required to be a string', escambo.parse({}))
    end)

    it('should reject missing content-type', function()
      local res = {get_header = function() end}
      assert.safefail('argument string is required to be a string', escambo.parse(res))
    end)

  end)

end)