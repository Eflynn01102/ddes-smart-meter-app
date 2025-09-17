package com.smartmeter.billing_service.domain.service;

import com.smartmeter.billing_service.domain.value.Interval;

import java.time.Instant;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.time.ZoneId;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

/**
 * Splits an interval at specified clock-time boundaries (e.g., 00:00, 07:00, 17:00, 22:00)
 * across day transitions in the given time zone.
 */
public final class IntervalSegmenter {
  private final ZoneId zone;

  public IntervalSegmenter(ZoneId zone) { this.zone = zone; }

  public List<Interval> splitByClockHours(Interval interval, List<LocalTime> boundaries) {
    var sorted = boundaries.stream().distinct().sorted().toList();
    var result = new ArrayList<Interval>();
    var cursor = interval.from();
    final var end = interval.to();

    while (cursor.isBefore(end)) {
      Instant nextBreak = end;

      // Consider next 2 days of boundaries to handle wrap-around
      var base = LocalDateTime.ofInstant(cursor, zone).toLocalDate();
      var candidates = new ArrayList<Instant>();
      for (int d = 0; d <= 2; d++) {
        var day = base.plusDays(d);
        for (var t : sorted) {
          var candidate = day.atTime(t).atZone(zone).toInstant();
          if (candidate.isAfter(cursor)) candidates.add(candidate);
        }
      }

      candidates.sort(Comparator.naturalOrder());
      for (var c : candidates) {
        if (c.isAfter(cursor)) { nextBreak = c.isBefore(end) ? c : end; break; }
      }

      var sliceEnd = nextBreak.isAfter(end) ? end : nextBreak;
      result.add(new Interval(cursor, sliceEnd));
      cursor = sliceEnd;
    }
    return result;
  }
}
