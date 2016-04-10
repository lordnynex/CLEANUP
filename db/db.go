package db

import (
	"fmt"
	"strconv"
	"strings"
	"time"

	"github.com/cheggaaa/pb"
	log "github.com/kotakanbe/go-cve-dictionary/log"

	"github.com/jinzhu/gorm"
	"github.com/k0kubun/pp"
	c "github.com/kotakanbe/go-cve-dictionary/config"
	jvn "github.com/kotakanbe/go-cve-dictionary/jvn"
	"github.com/kotakanbe/go-cve-dictionary/models"
	"github.com/kotakanbe/go-cve-dictionary/nvd"
)

var db *gorm.DB

// OpenDB opens Database
func OpenDB() (err error) {
	db, err = gorm.Open("sqlite3", c.Conf.DBPath)
	if err != nil {
		err = fmt.Errorf("Failed to open DB. datafile: %s, err: %s", c.Conf.DBPath, err)
		return

	}
	db.LogMode(c.Conf.DebugSQL)
	return
}

func recconectDB() error {
	var err error
	if err = db.Close(); err != nil {
		return fmt.Errorf("Failed to close DB. datafile: %s, err: %s", c.Conf.DBPath, err)
	}
	return OpenDB()
}

// MigrateDB migrates Database
func MigrateDB() error {
	if err := db.AutoMigrate(
		&models.CveDetail{},
		&models.Jvn{},
		&models.Nvd{},
		&models.Reference{},
		&models.Cpe{},
	).Error; err != nil {
		return fmt.Errorf("Failed to migrate. err: %s", err)
	}

	errMsg := "Failed to create index. err: %s"
	if err := db.Model(&models.CveDetail{}).
		AddIndex("idx_cve_detail_cveid", "cve_id").Error; err != nil {
		return fmt.Errorf(errMsg, err)
	}
	if err := db.Model(&models.Nvd{}).
		AddIndex("idx_nvds_cve_detail_id", "cve_detail_id").Error; err != nil {
		return fmt.Errorf(errMsg, err)
	}
	if err := db.Model(&models.Jvn{}).
		AddIndex("idx_jvns_cve_detail_id", "cve_detail_id").Error; err != nil {
		return fmt.Errorf(errMsg, err)
	}
	if err := db.Model(&models.Cpe{}).
		AddIndex("idx_cpes_jvn_id", "jvn_id").Error; err != nil {
		return fmt.Errorf(errMsg, err)
	}
	if err := db.Model(&models.Reference{}).
		AddIndex("idx_references_jvn_id", "jvn_id").Error; err != nil {
		return fmt.Errorf(errMsg, err)
	}
	if err := db.Model(&models.Cpe{}).
		AddIndex("idx_cpes_nvd_id", "nvd_id").Error; err != nil {
		return fmt.Errorf(errMsg, err)
	}
	if err := db.Model(&models.Cpe{}).
		AddIndex("idx_cpes_cpe_name", "cpe_name").Error; err != nil {
		return fmt.Errorf(errMsg, err)
	}
	if err := db.Model(&models.Reference{}).
		AddIndex("idx_references_nvd_id", "nvd_id").Error; err != nil {
		return fmt.Errorf(errMsg, err)
	}

	return nil
}

// Get Select Cve information from DB.
func Get(cveID string, priorityDB ...*gorm.DB) models.CveDetail {

	var conn *gorm.DB
	if len(priorityDB) == 1 {
		conn = priorityDB[0]
	} else {
		conn = db
	}

	c := models.CveDetail{}
	conn.Where(&models.CveDetail{CveID: cveID}).First(&c)

	if c.ID == 0 {
		return models.CveDetail{}
	}

	// JVN
	jvn := models.Jvn{}
	conn.Model(&c).Related(&jvn, "Jvn")
	c.Jvn = jvn

	if jvn.CveDetailID != 0 && jvn.ID != 0 {
		jvnRefs := []models.Reference{}
		conn.Model(&jvn).Related(&jvnRefs, "References")
		c.Jvn.References = jvnRefs

		// TODO commentout because JSON response size will be big. so Uncomment if needed.
		//  jvnCpes := []models.Cpe{}
		//  conn.Model(&jvn).Related(&jvnCpes, "Cpes")
		//  c.Jvn.Cpes = jvnCpes
	}

	// NVD
	nvd := models.Nvd{}
	conn.Model(&c).Related(&nvd, "Nvd")
	c.Nvd = nvd

	if nvd.CveDetailID != 0 && nvd.ID != 0 {
		nvdRefs := []models.Reference{}
		conn.Model(&nvd).Related(&nvdRefs, "References")
		c.Nvd.References = nvdRefs

		// TODO commentout because JSON response size will be big. so Uncomment if needed.
		//  nvdCpes := []models.Cpe{}
		//  conn.Model(&nvd).Related(&nvdCpes, "Cpes")
		//  c.Nvd.Cpes = nvdCpes
	}

	return c
}

// GetByCpeName Select Cve information from DB.
func GetByCpeName(cpeName string) (details []models.CveDetail) {
	cpes := []models.Cpe{}
	db.Where(&models.Cpe{CpeName: cpeName}).Find(&cpes)

	for _, cpe := range cpes {
		var cveDetailID uint
		if cpe.JvnID != 0 {
			//TODO test check CPE name format of JVN table
			jvn := models.Jvn{}
			db.Select("cve_detail_id").Where("ID = ?", cpe.JvnID).First(&jvn)
			cveDetailID = jvn.CveDetailID
		} else if cpe.NvdID != 0 {
			nvd := models.Nvd{}
			db.Select("cve_detail_id").Where("ID = ?", cpe.NvdID).First(&nvd)
			cveDetailID = nvd.CveDetailID
		}

		cveDetail := models.CveDetail{}
		db.Select("cve_id").Where("ID = ?", cveDetailID).First(&cveDetail)
		cveID := cveDetail.CveID
		details = append(details, Get(cveID))
	}
	return
}

// InsertJvn insert items fetched from JVN.
func InsertJvn(items []jvn.Item) error {
	log.Info("Inserting fetched CVEs...")

	cves := convertJvn(items)
	if err := insertIntoJvn(cves); err != nil {
		return err
	}
	return nil
}

// InsertIntoJvn inserts CveInformation into DB
func insertIntoJvn(cves []models.CveDetail) error {
	var refreshedJvns []string
	bar := pb.StartNew(len(cves))

	// TODO timeout, groutine, retry
	for chunked := range chunkSlice(cves, 100) {

		tx := db.Begin()
		for _, c := range chunked {

			bar.Increment()

			//TODO rename
			old := models.CveDetail{}

			// select old record.
			r := tx.Where(&models.CveDetail{CveID: c.CveID}).First(&old)
			//  pp.Println(r.Row())
			if r.RecordNotFound() || old.ID == 0 {
				if err := tx.Create(&c).Error; err != nil {
					tx.Rollback()
					return fmt.Errorf("Failed to insert. cve: %s, err: %s",
						pp.Sprintf("%v", c),
						err,
					)
				}
				refreshedJvns = append(refreshedJvns, c.CveID)
				continue
			}

			if !r.RecordNotFound() {

				// select Jvn from db
				jvn := models.Jvn{}
				db.Model(&old).Related(&jvn, "Jvn")

				if jvn.CveDetailID == 0 {
					c.Jvn.CveDetailID = old.ID
					if err := tx.Create(&c.Jvn).Error; err != nil {
						tx.Rollback()
						return fmt.Errorf("Failed to insert. cve: %s, err: %s",
							pp.Sprintf("%v", c.Jvn),
							err,
						)
					}
					refreshedJvns = append(refreshedJvns, c.CveID)
					continue
				}

				// Refresh to new JVN Record.

				// skip if the record has already been in DB and not modified.
				if jvn.LastModifiedDate.Equal(c.Jvn.LastModifiedDate) ||
					jvn.LastModifiedDate.After(c.Jvn.LastModifiedDate) {
					//  log.Debugf("Not modified. old: %s", old.CveID)
					continue
				} else {
					log.Debugf("newer Record found. CveID: %s, old: %s, new: %s",
						c.CveID,
						jvn.LastModifiedDate,
						c.Jvn.LastModifiedDate,
					)
				}

				// TODO commentout because db size will be big. so Uncomment if needed.
				// Delte old References
				refs := []models.Reference{}
				db.Model(&jvn).Related(&refs, "References")
				for _, r := range refs {
					if err := tx.Unscoped().Delete(r).Error; err != nil {
						tx.Rollback()
						return errDelete(c, err)
					}
				}

				// TODO commentout because db size will be big. so Uncomment if needed.
				// Delete old Cpes
				cpes := []models.Cpe{}
				db.Model(&jvn).Related(&cpes, "Cpes")
				for _, cpe := range cpes {
					if err := tx.Unscoped().Delete(cpe).Error; err != nil {
						tx.Rollback()
						return errDelete(c, err)
					}
				}

				// Delete old Jvn
				if err := tx.Unscoped().Delete(&jvn).Error; err != nil {
					tx.Rollback()
					return errDelete(c, err)
				}

				// Insert Jvn
				c.Jvn.CveDetailID = old.ID
				if err := tx.Create(&c.Jvn).Error; err != nil {
					tx.Rollback()
					return fmt.Errorf("Failed to insert. cve: %s, err: %s",
						pp.Sprintf("%v", c.Jvn),
						err,
					)
				}
				refreshedJvns = append(refreshedJvns, c.CveID)
			}
		}
		tx.Commit()
	}
	bar.Finish()
	log.Infof("Refreshed %d Jvns.", len(refreshedJvns))
	//  log.Debugf("%v", refreshedJvns)
	return nil
}

func errDelete(c models.CveDetail, err error) error {
	return fmt.Errorf("Failed to delete old record. cve: %s, err: %s",
		pp.Sprintf("%v", c),
		err,
	)
}

// ConvertJvn converts Jvn structure(got from JVN) to model structure.
func convertJvn(items []jvn.Item) (cves []models.CveDetail) {
	for _, item := range items {
		//TODO
		//  pp.Println(item.Cvss.Score)

		if item.Cvss.Score == "0" || len(item.Cvss.Score) == 0 {
			log.Debugf("Skip. CVSS Score is zero. JvnID: %s", item.Identifier)
			//ignore invalid item
			continue
		}

		// TODO commentout because db size will be big. so Uncomment if needed.
		//  References
		refs := []models.Reference{}
		for _, r := range item.References {
			ref := models.Reference{
				Source: r.Source,
				Link:   r.URL,
			}
			refs = append(refs, ref)
		}

		// TODO commentout because db size will be big. so Uncomment if needed.
		// Cpes
		cpes := []models.Cpe{}
		for _, c := range item.CpeItem {
			cpe, err := parseCpe(c.Name)
			if err != nil {
				panic(err)
			}
			cpes = append(cpes, cpe)
		}

		var publish, modified time.Time
		var err error
		if publish, err = toTime(item.Issued); err != nil {
			panic(err)
		}
		if modified, err = toTime(item.Issued); err != nil {
			panic(err)
		}

		var cveIDs []string
		var cveID string
		cveIDs = getCveIDs(item)
		if len(cveIDs) == 0 {
			log.Debugf("No CveIDs in references. JvnID: %s, Link: %s",
				item.Identifier,
				item.Link,
			)
			// ignore this item
			continue

		} else if 1 < len(cveIDs) {
			log.Debugf("Some CveIDs in references. JvnID: %s, Link: %s, CveIDs: %v",
				item.Identifier,
				item.Link,
				cveIDs,
			)
			// TODO ignore this item??
			continue
		}
		cveID = cveIDs[0]

		cve := models.CveDetail{
			CveID: cveID,

			// JVN
			Jvn: models.Jvn{
				Title:   item.Title,
				Summary: item.Description,
				JvnLink: item.Link,
				JvnID:   item.Identifier,

				Score:    stringToFloat(item.Cvss.Score),
				Severity: item.Cvss.Severity,
				Vector:   item.Cvss.Vector,
				//  Version:  item.Cvss.Version,

				// TODO commentout because db size will be big. so Uncomment if needed.
				References: refs,
				Cpes:       cpes,

				PublishedDate:    publish,
				LastModifiedDate: modified,
			},
		}
		cves = append(cves, cve)
	}
	return
}

func stringToFloat(str string) float64 {
	if len(str) == 0 {
		return 0
	}
	var f float64
	var ignorableError error
	if f, ignorableError = strconv.ParseFloat(str, 64); ignorableError != nil {
		log.Errorf("Failed to cast CVSS score. score: %s, err; %s",
			str,
			ignorableError,
		)
		f = 0
	}
	return f
}

// TODO move to jvn
func parseCpe(cpeName string) (models.Cpe, error) {
	s := strings.Split(cpeName, "cpe:/")
	if len(s) != 2 {
		return models.Cpe{}, fmt.Errorf("Unknow format: cpeName: %s", cpeName)
	}
	splitted := strings.Split(s[1], ":")
	items := []string{
		"", // Part
		"", // Vendor
		"", // Product
		"", // Version
		"", // Update
		"", // Edition
		"", // Language
	}
	for i, item := range splitted {
		items[i] = item
	}
	cpe := models.Cpe{
		CpeName:  cpeName,
		Part:     items[0],
		Vendor:   items[1],
		Product:  items[2],
		Version:  items[3],
		Update:   items[4],
		Edition:  items[5],
		Language: items[6],
	}
	return cpe, nil
}

// convert string time to time.Time
// JVN : "2016-01-26T13:36:23+09:00",
// NVD : "2016-01-20T21:59:01.313-05:00",
func toTime(strtime string) (t time.Time, err error) {
	layout := "2006-01-02T15:04:05-07:00"
	t, err = time.Parse(layout, strtime)
	if err != nil {
		return t, fmt.Errorf("Failed to parse time, time: %s, err: %s", strtime, err)
	}
	return
}

func getCveIDs(item jvn.Item) []string {
	cveIDsMap := map[string]bool{}
	for _, ref := range item.References {
		switch ref.Source {
		case "NVD", "CVE":
			cveIDsMap[ref.ID] = true
		}
	}
	var cveIDs []string
	for cveID := range cveIDsMap {
		cveIDs = append(cveIDs, cveID)
	}
	return cveIDs
}

// convertNvd converts Nvd structure(got from NVD) to model structure.
func convertNvd(entries []nvd.Entry) (cves []models.CveDetail) {
	for _, entry := range entries {
		//  if entry.Cvss.Score == "0" || len(entry.Cvss.Score) == 0 {
		//      log.Warnf("Skip. CVSS Score is zero. JvnID: %s", entry.CveID)
		//      //ignore invalid item
		//      continue
		//  }

		// TODO commentout because db size will be big. so Uncomment if needed.
		// References
		refs := []models.Reference{}
		for _, r := range entry.References {
			ref := models.Reference{
				Source: r.Source,
				Link:   r.Link.Href,
			}
			refs = append(refs, ref)
		}

		// TODO commentout because db size will be big. so Uncomment if needed.
		//  // Cpes
		cpes := []models.Cpe{}
		for _, c := range entry.Products {
			cpe, err := parseCpe(c)
			if err != nil {
				panic(err)
			}
			cpes = append(cpes, cpe)
		}

		cve := models.CveDetail{
			CveID: entry.CveID,
			Nvd: models.Nvd{
				Summary:               entry.Summary,
				Score:                 stringToFloat(entry.Cvss.Score),
				AccessVector:          entry.Cvss.AccessVector,
				AccessComplexity:      entry.Cvss.AccessComplexity,
				Authentication:        entry.Cvss.Authentication,
				ConfidentialityImpact: entry.Cvss.ConfidentialityImpact,
				IntegrityImpact:       entry.Cvss.IntegrityImpact,
				AvailabilityImpact:    entry.Cvss.AvailabilityImpact,
				PublishedDate:         entry.PublishedDate,
				LastModifiedDate:      entry.LastModifiedDate,

				// TODO commentout because db size will be big. so Uncomment if needed.
				Cpes:       cpes,
				References: refs,
			},
		}
		cves = append(cves, cve)
	}
	return
}

func chunkSlice(l []models.CveDetail, n int) chan []models.CveDetail {
	ch := make(chan []models.CveDetail)

	go func() {
		for i := 0; i < len(l); i += n {
			fromIdx := i
			toIdx := i + n
			if toIdx > len(l) {
				toIdx = len(l)
			}
			ch <- l[fromIdx:toIdx]
		}
		close(ch)
	}()
	return ch
}

// CountNvd count nvd table
func CountNvd() (int, error) {
	var count int
	if err := db.Model(&models.Nvd{}).Count(&count).Error; err != nil {
		return 0, err
	}
	return count, nil
}

// InsertNvd inserts CveInformation into DB
func InsertNvd(entries []nvd.Entry) error {
	log.Info("Inserting CVEs...")

	cves := convertNvd(entries)
	if err := insertIntoNvd(cves); err != nil {
		return err
	}
	return nil
}

// insertIntoNvd inserts CveInformation into DB
func insertIntoNvd(cves []models.CveDetail) error {
	refreshedNvds := []string{}
	bar := pb.StartNew(len(cves))

	for chunked := range chunkSlice(cves, 100) {
		tx := db.Begin()
		for _, c := range chunked {
			bar.Increment()

			//TODO rename
			old := models.CveDetail{}

			// select old record.
			r := tx.Where(&models.CveDetail{CveID: c.CveID}).First(&old)
			if r.RecordNotFound() || old.ID == 0 {
				if err := tx.Create(&c).Error; err != nil {
					tx.Rollback()
					return fmt.Errorf("Failed to insert. cve: %s, err: %s",
						pp.Sprintf("%v", c),
						err,
					)
				}
				refreshedNvds = append(refreshedNvds, c.CveID)
				continue
			}

			if !r.RecordNotFound() {

				// select Nvd from db
				nvd := models.Nvd{}
				db.Model(&old).Related(&nvd, "Nvd")

				if nvd.CveDetailID == 0 {
					c.Nvd.CveDetailID = old.ID
					if err := tx.Create(&c.Nvd).Error; err != nil {
						tx.Rollback()
						return fmt.Errorf("Failed to insert. cve: %s, err: %s",
							pp.Sprintf("%v", c.Nvd),
							err,
						)
					}
					refreshedNvds = append(refreshedNvds, c.CveID)
					continue
				}

				// Refresh to new NVD Record.

				// skip if the record has already been in DB and not modified.
				if nvd.LastModifiedDate.Equal(c.Nvd.LastModifiedDate) ||
					nvd.LastModifiedDate.After(c.Nvd.LastModifiedDate) {
					//  log.Debugf("Not modified. old: %s", old.CveID)
					continue
				} else {
					log.Debugf("newer Record found. CveID: %s, old: %s, new: %s",
						c.CveID,
						nvd.LastModifiedDate,
						c.Nvd.LastModifiedDate,
					)
				}

				// TODO commentout because db size will be big. so Uncomment if needed.
				// Delte old References
				refs := []models.Reference{}
				db.Model(&nvd).Related(&refs, "References")
				for _, r := range refs {
					if err := tx.Unscoped().Delete(r).Error; err != nil {
						tx.Rollback()
						return errDelete(c, err)
					}
				}

				// TODO commentout because db size will be big. so Uncomment if needed.
				//  // Delete old Cpes
				cpes := []models.Cpe{}
				db.Model(&nvd).Related(&cpes, "Cpes")
				for _, cpe := range cpes {
					if err := tx.Unscoped().Delete(cpe).Error; err != nil {
						tx.Rollback()
						return errDelete(c, err)
					}
				}

				// Delete old Nvd
				if err := tx.Unscoped().Delete(&nvd).Error; err != nil {
					tx.Rollback()
					return errDelete(c, err)
				}

				// Insert Nvd
				c.Nvd.CveDetailID = old.ID
				if err := tx.Create(&c.Nvd).Error; err != nil {
					tx.Rollback()
					return fmt.Errorf("Failed to insert. cve: %s, err: %s",
						pp.Sprintf("%v", c.Nvd),
						err,
					)
				}
				refreshedNvds = append(refreshedNvds, c.CveID)
			}
		}
		tx.Commit()
	}
	bar.Finish()

	log.Infof("Refreshed %d Nvds.", len(refreshedNvds))
	//  log.Debugf("%v", refreshedNvds)
	return nil
}
