
require [[spec.helper]]

describe('escambo.format(object)', function()

  local escambo

  setup(function()
    escambo = require('..escambo')()
  end)

  teardown(function()
    escambo = nil
  end)

  it('should format basic type', function()
    assert.safeequals('text/html', escambo.format({type='text/html'}))
    assert.safeequals('text/html', escambo.format({type='text', subtype='html'}))
  end)

  it('should format type with suffix', function()
    assert.safeequals('image/svg+xml', escambo.format({type='image/svg+xml'}))
    assert.safeequals('image/svg+xml', escambo.format({type='image', subtype='svg+xml'}))
    assert.safeequals('image/svg+xml', escambo.format({type='image', subtype='svg', suffix='xml'}))
  end)

  it('should format type with parameter', function()
    assert.safeequals(
      'text/html; charset=utf-8', escambo.format({type='text/html', parameters={charset='utf-8'}})
    )
    assert.safeequals(
      'text/html; charset=utf-8',
      escambo.format({type='text', subtype='html', parameters={charset='utf-8'}})
    )
  end)

  it('should format type with parameter that needs quotes', function()
    assert.safeequals(
      'text/html; foo="bar or \\"baz\\""',
      escambo.format({type='text/html', parameters={foo='bar or "baz"'}})
    )
    assert.safeequals(
      'text/html; foo="bar or \\"baz\\""',
      escambo.format({type='text', subtype='html', parameters={foo='bar or "baz"'}})
    )
  end)

  it('should format type with parameter with empty value', function()
    assert.safeequals(
      'text/html; foo=""', escambo.format({type='text/html', parameters={foo=''}})
    )
    assert.safeequals(
      'text/html; foo=""', escambo.format({type='text', subtype='html', parameters={foo=''}})
    )
  end)

  it('should format type with multiple parameters', function()
    assert.safeequals(
      'text/html; bar=baz; charset=utf-8; foo=bar',
      escambo.format({type='text/html', parameters={charset='utf-8', foo='bar', bar='baz'}})
    )
    assert.safeequals(
      'text/html; bar=baz; charset=utf-8; foo=bar',
      escambo.format({type='text', subtype='html', parameters={charset='utf-8', foo='bar', bar='baz'}})
    )
  end)

  it('should require argument', function()
    assert.safefail('argument is required', escambo.format())
  end)

  it('should reject non-objects', function()
    assert.safefail('argument is required', escambo.format())
  end)

  it('should require type', function()
    assert.safefail('invalid type', escambo.format({}))
  end)

  it('should reject invalid type', function()
    assert.safefail('invalid type', escambo.format({type='text/'}))
  end)

  it('should reject invalid type with LWS', function()
    assert.safefail('invalid type', escambo.format({type=' text/html'}))
  end)

  it('should reject invalid parameter name', function()
    assert.safefail(
      'invalid parameter name', escambo.format({type='image/svg', parameters={['foo/']='bar'}})
    )
  end)

  it('should reject invalid parameter value', function()
    assert.safefail(
      'invalid parameter value', escambo.format({type='image/svg', parameters={foo='bar\0'}})
    )
  end)

end)