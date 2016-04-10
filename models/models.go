package models

import (
	"fmt"
	"time"

	"github.com/jinzhu/gorm"
	log "github.com/kotakanbe/go-cve-dictionary/log"
)

// CveDetails is for sorting
type CveDetails []CveDetail

func (c CveDetails) Len() int {
	return len(c)
}

func (c CveDetails) Swap(i, j int) {
	c[i], c[j] = c[j], c[i]
}

func (c CveDetails) Less(i, j int) bool {
	return c[i].CveID < c[j].CveID
}

// CveDetail is a parent of Jnv/Nvd model
type CveDetail struct {
	gorm.Model
	CveInfoID uint // Foreign key

	CveID string
	Nvd   Nvd
	Jvn   Jvn
}

// CvssScore returns CVSS Score of the CVE
func (c CveDetail) CvssScore(lang string) float64 {
	switch lang {
	case "en":
		if c.Nvd.GetID() != 0 && c.Nvd.CvssScore() != 0 {
			log.Debugf("%s, Score :%f, Nvd.ID: %d, Lang: %s",
				c.CveID,
				c.Nvd.CvssScore(),
				c.Nvd.ID,
				lang)
			return c.Nvd.CvssScore()
		} else if c.Jvn.GetID() != 0 && c.Jvn.CvssScore() != 0 {
			log.Debugf("%s, Score :%f, Jvn.ID: %d, Lang: %s",
				c.CveID,
				c.Jvn.CvssScore(),
				c.Jvn.ID,
				lang)
			return c.Jvn.CvssScore()
		} else {
			log.Debugf("Cvss Score is unknown. CveID: %v",
				c.Jvn.JvnID,
				c.Jvn.Link(),
				c.CveID,
			)
		}
		return -1
	case "ja":
		if c.Jvn.GetID() != 0 && c.Jvn.CvssScore() != 0 {
			log.Debugf("%s, Score :%f, Jvn.ID: %d, Lang: %s",
				c.CveID,
				c.Jvn.CvssScore(),
				c.Jvn.GetID(),
				lang)
			return c.Jvn.CvssScore()
		} else if c.Nvd.GetID() != 0 && c.Nvd.CvssScore() != 0 {
			log.Debugf("%s, Score :%f, Nvd.ID: %d, Lang: %s",
				c.CveID,
				c.Nvd.CvssScore(),
				c.Nvd.ID,
				lang)
			return c.Nvd.CvssScore()
		} else {
			log.Debugf("Cvss Score is unknown. CveID: %v",
				c.Jvn.JvnID,
				c.Jvn.Link(),
				c.CveID,
			)
		}
		return -1
	default:
		log.Errorf("Not implement yet. lang: %s", lang)
		return c.CvssScore("en")
		// reutrn -1
	}
}

// CvssV2CalculatorLink returns cvss Caluculate site URL.
// https://nvd.nist.gov/cvss/v2-calculator?name=CVE-2015-5477&vector=(AV:N/AC:L/Au:N/C:N/I:N/A:C)
func (c CveDetail) CvssV2CalculatorLink(lang string) string {
	var vector string
	switch lang {
	case "en":
		vector = c.Nvd.CvssVector()
	case "ja":
		vector = c.Jvn.CvssVector()
	default:
		return ""
	}
	return fmt.Sprintf("https://nvd.nist.gov/cvss/v2-calculator?name=%s&vector=%s",
		c.CveID,
		vector)
}

// CveDictionary is interface of JVN, NVD
type CveDictionary interface {
	GetID() uint
	GetCveDetailID() uint
	SetCveDetailID(id uint)
	CveTitle() string
	CveSummary() string
	CvssScore() float64
	CvssVector() string
	CvssSeverity() string
	Link() string
	VulnSiteReferences() []Reference
	SetVulnSiteReferences([]Reference)
	SetCpes([]Cpe)
	LastModified() time.Time
}

// Nvd is a model of NVD
type Nvd struct {
	gorm.Model
	CveDetailID uint

	Summary string

	Score                 float64 // 1 to 10
	AccessVector          string
	AccessComplexity      string
	Authentication        string
	ConfidentialityImpact string
	IntegrityImpact       string
	AvailabilityImpact    string

	Cpes       []Cpe
	References []Reference

	PublishedDate    time.Time
	LastModifiedDate time.Time
}

// GetID return title
func (c Nvd) GetID() uint {
	return c.ID
}

// GetCveDetailID return title
func (c Nvd) GetCveDetailID() uint {
	return c.CveDetailID
}

// SetCveDetailID return title
func (c *Nvd) SetCveDetailID(id uint) {
	c.CveDetailID = id
}

//CveTitle return title
func (c Nvd) CveTitle() string {
	return ""
}

//CveSummary return summary
func (c Nvd) CveSummary() string {
	return c.Summary
}

//CvssScore return title
func (c Nvd) CvssScore() float64 {
	return c.Score
}

// Severity return severity ranking that NVD povided.
// https://nvd.nist.gov/cvss.cfm
func (c Nvd) Severity() string {
	switch {
	case 7 <= c.CvssScore():
		return "High"
	case 4 <= c.CvssScore() && c.CvssScore() < 7:
		return "Medium"
	case 0 < c.CvssScore() && c.CvssScore() < 4:
		return "Low"
	default:
		return "?"
	}
}

// CvssVector make CVSS Vector string
// https://nvd.nist.gov/cvss.cfm?vectorinfo&version=2
//  Example 1: (AV:L/AC:H/Au:N/C:N/I:P/A:C)
//  Example 2: (AV:A/AC:L/Au:M/C:C/I:N/A:P)
func (c Nvd) CvssVector() string {
	return fmt.Sprintf("(AV:%s/AC:%s/Au:%s/C:%s/I:%s/A:%s)",
		firstChar(c.AccessVector),
		firstChar(c.AccessComplexity),
		firstChar(c.Authentication),
		firstChar(c.ConfidentialityImpact),
		firstChar(c.IntegrityImpact),
		firstChar(c.AvailabilityImpact),
	)
}

func firstChar(str string) string {
	if len(str) == 0 {
		return "?"
	}
	return string(str[0])
}

// Link return summary
func (c Nvd) Link() string {
	//TODO return NVD Link
	return ""
}

// VulnSiteReferences return References
func (c Nvd) VulnSiteReferences() []Reference {
	//TODO return NVD Link
	return c.References
}

// SetVulnSiteReferences set References
func (c Nvd) SetVulnSiteReferences(r []Reference) {
	c.References = r
}

// SetCpes set cpes
func (c *Nvd) SetCpes(r []Cpe) {
	c.Cpes = r
}

// LastModified get LastModifiedDate
func (c Nvd) LastModified() time.Time {
	return c.LastModifiedDate
}

// Jvn is a model of JVN
type Jvn struct {
	gorm.Model
	CveDetailID uint

	Title   string
	Summary string
	JvnLink string
	JvnID   string

	Score    float64 // 1 to 10
	Severity string  // Low|Medium|High
	Vector   string  // (AV:N/AC:M/Au:N/C:N/I:P/A:N)
	//  Version   string  // 2.0

	References []Reference
	Cpes       []Cpe

	PublishedDate    time.Time
	LastModifiedDate time.Time
}

// GetID return title
func (c Jvn) GetID() uint {
	return c.ID
}

// SetCveDetailID return title
func (c *Jvn) SetCveDetailID(id uint) {
	c.CveDetailID = id
}

// GetCveDetailID return title
func (c Jvn) GetCveDetailID() uint {
	return c.CveDetailID
}

//CveTitle return title
func (c Jvn) CveTitle() string {
	return c.Title
}

// CveSummary return summary
func (c Jvn) CveSummary() string {
	return c.Summary
}

//CvssScore return severity ranking
func (c Jvn) CvssScore() float64 {
	return c.Score
}

//CvssVector return severity ranking
func (c Jvn) CvssVector() string {
	return c.Vector
}

// Link return summary
func (c Jvn) Link() string {
	return c.JvnLink
}

//CvssSeverity return severity ranking
func (c Jvn) CvssSeverity() string {
	return c.Severity
}

// VulnSiteReferences return summary
func (c Jvn) VulnSiteReferences() []Reference {
	//TODO return NVD Link
	return c.References
}

// SetVulnSiteReferences set References
func (c *Jvn) SetVulnSiteReferences(r []Reference) {
	c.References = r
}

// SetCpes set cpes
func (c *Jvn) SetCpes(r []Cpe) {
	c.Cpes = r
}

// LastModified set cpes
func (c Jvn) LastModified() time.Time {
	return c.LastModifiedDate
}

// Cpe is Child model of Jvn/Nvd.
// see https://www.ipa.go.jp/security/vuln/CPE.html
type Cpe struct {
	gorm.Model
	JvnID uint
	NvdID uint

	// CPE Name (URL sytle)
	// JVN ... cpe:/a:oracle:mysql
	// NVD ... cpe:/a:cisco:unified_contact_center_express:11.0%281%29
	CpeName string

	// each items
	Part     string
	Vendor   string
	Product  string
	Version  string
	Update   string
	Edition  string
	Language string
}

// Reference is Child model of Jvn/Nvd.
// It holds reference information about the CVE.
type Reference struct {
	gorm.Model
	JvnID uint
	NvdID uint

	Source string
	Link   string
}
